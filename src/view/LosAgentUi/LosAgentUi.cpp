// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentUi.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/LosAgentKeyUi/LosAgentKeyUi.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>

namespace
{
    // 把文本编码成合法的 JS 字符串字面量 (JSON 转义 \n \r \" \\ 等).
    // 之前直接拼进单引号字符串, 回复一旦含换行就产生 JS 语法错误,
    // runJavaScript 静默失败, 表现为"发送消息没有回".
    QString jsString(const QString &s)
    {
        // 注意: QJsonDocument::fromVariant(QString) 生成的是"标量文档", 对其 toJson() 会返回空字符串!
        // 因此必须用数组包裹: toJson 得到 ["..."] 后再剥掉首尾方括号, 才是合法的 JS 字符串字面量.
        QJsonArray arr;
        arr.append(s);
        const QByteArray json = QJsonDocument(arr).toJson(QJsonDocument::Compact);
        return QString::fromUtf8(json).mid(1, json.size() - 2);
    }
} // namespace

namespace LosView
{

    LosAgentBridge::LosAgentBridge(LosAgentUi *ui, QObject *parent) : QObject(parent), L_ui(ui) {}

    void LosAgentBridge::sendMessage(const QString &text)
    {
        if (L_ui)
            L_ui->onUserSend(text);
    }



    void LosAgentBridge::providerChanged(const QString &name)
    {
        if (L_ui)
            L_ui->onProviderChanged(name);
    }



    void LosAgentBridge::addProvider()
    {
        if (L_ui)
            L_ui->onAddClicked();
    }



    void LosAgentBridge::refreshProviders()
    {
        if (L_ui)
            L_ui->onRefreshProviders();
    }



    void LosAgentBridge::deleteProvider()
    {
        if (L_ui)
            L_ui->onDeleteProvider();
    }



    void LosAgentBridge::deleteModel()
    {
        if (L_ui)
            L_ui->onDeleteModel();
    }



    void LosAgentBridge::initState()
    {
        if (L_ui)
            L_ui->onInitState();
    }



    LosAgentUi::LosAgentUi(QWidget *parent) : QWidget(parent)
    {
        initUi();
        initConnect();
    }



    LosAgentUi::~LosAgentUi() {}

    void LosAgentUi::runJs(const QString &js)
    {
        if (L_webView)
            L_webView->page()->runJavaScript(js);
    }



    void LosAgentUi::initUi()
    {
        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        L_webView = new QWebEngineView(this);
        L_channel = new QWebChannel(this);
        L_bridge  = new LosAgentBridge(this, this);

        L_channel->registerObject(QStringLiteral("bridge"), L_bridge);
        L_webView->page()->setWebChannel(L_channel);
        L_webView->setUrl(QUrl(QStringLiteral("qrc:/web/web/losagent.html")));
        layout->addWidget(L_webView);

        // Streaming flush throttle timer: every 33ms (~30fps) pushes accumulated
        // text to the Web view in one batch, avoiding per-chunk runJavaScript + DOM
        // reflow that makes QWebEngineView laggy.
        L_flushTimer = new QTimer(this);
        L_flushTimer->setInterval(33);
        L_flushTimer->setSingleShot(true);
        connect(L_flushTimer, &QTimer::timeout, this, &LosAgentUi::flushChunks);
    }



    void LosAgentUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_agent_listProviders_response, this, &LosAgentUi::onProvidersReceived);
        connect(&router, &LosCore::LosRouter::_cmd_agent_addProvider_response, this, &LosAgentUi::onProviderAdded);
        connect(&router, &LosCore::LosRouter::_cmd_agent_deleteProvider_response, this, &LosAgentUi::onProviderDeleted);
        connect(&router, &LosCore::LosRouter::_cmd_agent_reply, this,
                [this](bool ok, const QString &msg)
                {
                    if (ok)
                        onAgentReply(msg);
                    else
                        onAgentError(msg);
                });
        connect(&router, &LosCore::LosRouter::_cmd_agent_replyChunk, this, &LosAgentUi::onReplyChunk);
        connect(&router, &LosCore::LosRouter::_cmd_agent_replyDone, this, &LosAgentUi::onReplyDone);
        connect(&router, &LosCore::LosRouter::_cmd_auth_loginStateChanged, this,
                [this](bool loggedIn)
                {
                    if (loggedIn)
                        loadProviders();
                });
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &) { applyThemeToWeb(); });
        connect(L_webView->page(), &QWebEnginePage::loadFinished, this,
                [this](bool ok)
                {
                    if (ok)
                    {
                        L_pageReady = true;
                        applyThemeToWeb(); // 页面就绪后再应用主题, 避免启动时序竞争
                    }
                });
        loadProviders();
    }



    void LosAgentUi::applyThemeToWeb()
    {
        if (!L_pageReady)
            return; // 页面脚本尚未加载, 避免 applyTheme is not defined; 加载完成后会重新调用
        auto &theme           = LosCore::LosThemeManager::instance();
        const QString name    = theme.currentTheme();
        const auto tokens     = theme.uiTokens(name);
        const QString bg      = tokens.value(QStringLiteral("background"), QStringLiteral("#282a36"));
        const QString panel   = tokens.value(QStringLiteral("panelBg"), QStringLiteral("#21222c"));
        const QString fg      = tokens.value(QStringLiteral("foreground"), QStringLiteral("#f8f8f2"));
        const QString muted   = tokens.value(QStringLiteral("muted"), QStringLiteral("#6272a4"));
        const QString prim    = tokens.value(QStringLiteral("primary"), QStringLiteral("#bd93f9"));
        const QString border  = tokens.value(QStringLiteral("borderStrong"), QStringLiteral("#191a21"));
        const QString sel     = tokens.value(QStringLiteral("selection"), QStringLiteral("#44475a"));
        const QString hiFg    = tokens.value(QStringLiteral("highlightFg"), QStringLiteral("#ffffff"));
        const QString success = tokens.value(QStringLiteral("success"), QStringLiteral("#50fa7b"));
        runJs(QStringLiteral("applyTheme('%1','%2','%3','%4','%5','%6','%7','%8','%9')").arg(bg, panel, fg, muted, prim, border, sel, hiFg, success));
    }



    void LosAgentUi::loadProviders()
    {
        emit LosCore::LosRouter::instance()._cmd_agent_listProviders_request();
    }



    /**
     * @brief 
     * 
     */
    void LosAgentUi::onInitState()
    {
        applyThemeToWeb();
        loadProviders();
    }



    /**
     * @brief onProvidersReceived
     * 
     * @param ok 
     * @param providerModels 
     * @param msg 
     */
    void LosAgentUi::onProvidersReceived(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg)
    {
        Q_UNUSED(msg);
        if (!ok)
            return;
        L_providerModels      = providerModels;
        QStringList providers = providerModels.keys();
        if (providers.isEmpty())
        {
            // 厂商全部被删光:
            // 复位当前选择并清空下拉框
            L_currentProvider.clear();
            L_currentModel.clear();
            setProviderModels({}, {});
            return;
        }
        if (L_currentProvider.isEmpty() || !providerModels.contains(L_currentProvider))
            L_currentProvider = providers.first();
        QStringList models = providerModels.value(L_currentProvider);
        if (models.isEmpty())
            return;
        if (L_currentModel.isEmpty() || !models.contains(L_currentModel))
            L_currentModel = models.first();
        setProviderModels(providers, models);
    }



    /**
     * @brief onProviderChanged
     * 
     * @param name 
     */
    void LosAgentUi::onProviderChanged(const QString &name)
    {
        if (name.isEmpty() || !L_providerModels.contains(name))
            return;
        L_currentProvider  = name;
        QStringList models = L_providerModels.value(name);
        if (-1 == models.indexOf(L_currentModel))
            L_currentModel = models.isEmpty() ? QString() : models.first();
        setProviderModels(L_providerModels.keys(), models);
    }



    /**
     * @brief onProviderChanged
     * 
     * @param index 
     */
    void LosAgentUi::onProviderChanged(int index)
    {
        Q_UNUSED(index);
    }



    /**
     * @brief onAddClicked
     */
    void LosAgentUi::onAddClicked()
    {
        LosAgentKeyUi dialog(this);
        dialog.exec();
        loadProviders();
    }



    /**
     * @brief 
     * 
     */
    void LosAgentUi::onRefreshProviders()
    {
        loadProviders();
    }



    void LosAgentUi::onDeleteModel()
    {
        if (L_currentModel.isEmpty())
        {
            runJs(QStringLiteral("addMessage('left',%1)").arg(jsString(QStringLiteral("[错误] 当前没有可删除的模型"))));
            return;
        }
        emit LosCore::LosRouter::instance()._cmd_agent_deleteProvider_request(L_currentProvider, L_currentModel);
    }



    void LosAgentUi::onDeleteProvider()
    {
        if (L_currentProvider.isEmpty())
        {
            runJs(QStringLiteral("addMessage('left',%1)").arg(jsString(QStringLiteral("[错误] 当前没有可删除的厂商"))));
            return;
        }
        emit LosCore::LosRouter::instance()._cmd_agent_deleteProvider_request(L_currentProvider, QString());
    }



    void LosAgentUi::onProviderDeleted(bool ok, const QString &message)
    {
        if (ok)
        {
            loadProviders();
            return;
        }
        runJs(QStringLiteral("addMessage('left',%1)").arg(jsString(QStringLiteral("[错误] ") + message)));
    }



    void LosAgentUi::onProviderAdded(bool success, const QString &message)
    {
        Q_UNUSED(message);
        if (success)
            loadProviders();
    }



    void LosAgentUi::setProviderModels(const QStringList &providers, const QStringList &models)
    {
        QJsonArray pa, ma;
        for (const auto &p : providers)
            pa.append(p);
        for (const auto &m : models)
            ma.append(m);
        const QString js =
            QStringLiteral("updateProviderModel(%1,%2,%3,%4)")
                .arg(QString::fromUtf8(QJsonDocument(pa).toJson(QJsonDocument::Compact)),
                     QString::fromUtf8(QJsonDocument(ma).toJson(QJsonDocument::Compact)), jsString(L_currentProvider), jsString(L_currentModel));
        runJs(js);
    }



    void LosAgentUi::onUserSend(const QString &text)
    {
        const QString t = text.trimmed();
        if (t.isEmpty())
            return;
        if (L_currentProvider.isEmpty() || L_currentModel.isEmpty())
        {
            runJs(QStringLiteral("addMessage('left',%1)").arg(jsString(QStringLiteral("[错误] 尚未选择厂商/模型, 请先添加厂商或刷新模型列表"))));
            return;
        }
        emit LosCore::LosRouter::instance()._cmd_agent_sendMessage(t, L_currentProvider, L_currentModel);
        L_streamingBuffer.clear();
        L_pendingChunk.clear();
        L_flushTimer->stop();
        runJs(QStringLiteral("addMessage('left','')"));
    }



    void LosAgentUi::onAgentReply(const QString &message)
    {
        runJs(QStringLiteral("updateLastLeftText(%1)").arg(jsString(message)));
    }



    void LosAgentUi::onAgentError(const QString &message)
    {
        runJs(QStringLiteral("updateLastLeftText(%1)").arg(jsString(QStringLiteral("[错误] ") + message)));
    }



    void LosAgentUi::onReplyChunk(const QString &data)
    {
        // Accumulate chunk and start throttle timer.
        // Timer fires once and pushes the accumulated buffer to the Web view,
        // avoiding per-chunk runJavaScript that blocks the UI thread.
        L_streamingBuffer += data;
        L_pendingChunk += data;
        if (!L_flushTimer->isActive())
            L_flushTimer->start();
    }



    void LosAgentUi::flushChunks()
    {
        // Timer expired: push everything accumulated since last flush.
        if (L_pendingChunk.isEmpty())
            return;
        runJs(QStringLiteral("updateLastLeftText(%1)").arg(jsString(L_streamingBuffer)));
        L_pendingChunk.clear();
    }



    void LosAgentUi::onReplyDone()
    {
        // Stream ended: flush remaining content immediately, then reset buffers.
        L_flushTimer->stop();
        if (!L_pendingChunk.isEmpty())
        {
            runJs(QStringLiteral("updateLastLeftText(%1)").arg(jsString(L_streamingBuffer)));
            L_pendingChunk.clear();
        }
        L_streamingBuffer.clear();
    }

} /* namespace LosView */

#include "LosAgentUi.moc"