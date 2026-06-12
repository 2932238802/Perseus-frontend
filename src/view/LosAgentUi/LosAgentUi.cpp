// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosAgentUi.h"
#include "./ui_LosAgentUi.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"
#include "view/LosAgentKeyUi/LosAgentKeyUi.h"
#include "view/style/LosAgent_style.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QTextBrowser>
#include <QTextDocument>

namespace LosView
{
    /**
     * @brief 构造函数
     * - 构建控件 -> 着色 -> 连接信号
     */
    LosAgentUi::LosAgentUi(QWidget *parent) : QWidget(parent), ui(new Ui::LosAgentUi)
    {
        initUi();
        initStyle();
        initConnect();
    }
    LosAgentUi::~LosAgentUi()
    {
        delete ui;
    }



    /**
     * @brief   装载 .ui 表单
     */
    void LosAgentUi::initUi()
    {
        ui->setupUi(this);
    }



    /**
     * @brief loadProviders
     * - 向后端请求当前用户的厂商/模型配置 (list_providers)
     * - 真正的填充在 onProvidersReceived 中完成
     */
    void LosAgentUi::loadProviders()
    {
        emit LosCore::LosRouter::instance()._cmd_agent_listProviders_request();
    }



    /**
     * @brief onProvidersReceived
     * - 收到后端配置 -> 缓存映射 -> 填充厂商下拉 -> 联动模型下拉
     */
    void LosAgentUi::onProvidersReceived(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg)
    {
        Q_UNUSED(msg);
        if (!ok)
            return;
        L_providerModels = providerModels;
        ui->provider_combo->blockSignals(true);
        ui->provider_combo->clear();
        for (auto it = L_providerModels.constBegin(); it != L_providerModels.constEnd(); ++it)
            ui->provider_combo->addItem(it.key());
        ui->provider_combo->blockSignals(false);
        onProviderChanged(ui->provider_combo->currentIndex());
    }



    /**
     * @brief onProviderChanged
     * - 厂商切换 -> 从缓存映射取出该厂商模型, 刷新模型下拉
     */
    void LosAgentUi::onProviderChanged(int index)
    {
        Q_UNUSED(index);
        const QString provider = ui->provider_combo->currentText();
        ui->model_combo->clear();
        if (provider.isEmpty())
            return;
        const QStringList models = L_providerModels.value(provider);
        for (const QString &m : models)
            ui->model_combo->addItem(m);
    }



    /**
     * @brief onAddClicked
     * - 弹出添加 AI 配置对话框
     */
    void LosAgentUi::onAddClicked()
    {
        LosAgentKeyUi dialog(this);
        dialog.exec();
    }



    /**
     * @brief onProviderAdded
     * - 新增厂商配置成功后 -> 重新拉取一次, 刷新下拉
     */
    void LosAgentUi::onProviderAdded(bool success, const QString &message)
    {
        Q_UNUSED(message);
        if (success)
            loadProviders();
    }



    /**
     * @brief initStyle
     * - 用当前主题着色
     */
    void LosAgentUi::initStyle()
    {
        applyTheme(LosCore::LosThemeManager::instance().currentTheme());
    }



    /**
     * @brief applyTheme
     * - 主题换肤回调 (与 LosAuthUi 约定一致)
     * - 把 LosAgent_style.h 模板里的 @token@ 替换为当前主题色后应用
     */
    void LosAgentUi::applyTheme(const QString &themeName)
    {
        const QString qss = LosCore::LosThemeManager::instance().buildExtraQss(LosStyle::losAgent_getStyleTemplate(), themeName);
        setStyleSheet(qss);
    }



    /**
     * @brief initConnect
     * - 发送按钮 / 回车提交 / Agent 回包 / 主题换肤
     */
    void LosAgentUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(ui->send_btn, &QPushButton::clicked, this, &LosAgentUi::onSendClicked);
        connect(ui->input_edit, &QLineEdit::returnPressed, this, &LosAgentUi::onSendClicked);
        connect(ui->add_btn, &QPushButton::clicked, this, &LosAgentUi::onAddClicked);
        connect(ui->refresh_btn, &QPushButton::clicked, this, &LosAgentUi::loadProviders);
        connect(ui->provider_combo, &QComboBox::currentIndexChanged, this, &LosAgentUi::onProviderChanged);
        connect(&router, &LosCore::LosRouter::_cmd_agent_listProviders_response, this, &LosAgentUi::onProvidersReceived);
        connect(&router, &LosCore::LosRouter::_cmd_agent_addProvider_response, this, &LosAgentUi::onProviderAdded);
        // 重新登录成功后自动刷新配置
        connect(&router, &LosCore::LosRouter::_cmd_auth_loginStateChanged, this,
                [this](bool loggedIn)
                {
                    if (loggedIn)
                        loadProviders();
                });
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this, [this](const QString &name) { applyTheme(name); });
        connect(&router, &LosCore::LosRouter::_cmd_agent_reply, this,
                [this](bool ok, const QString &msg)
                {
                    if (ok)
                    {
                        onAgentReply(msg);
                    }
                    else
                    {
                        onAgentError(msg);
                    }
                });
        loadProviders();
    }



    /**
     * @brief onSendClicked
     * - 立刻显示用户气泡 -> 把消息抛给 core (LosAgentManager)
     */
    void LosAgentUi::onSendClicked()
    {
        const QString text = ui->input_edit->text().trimmed();
        if (text.isEmpty())
            return;
        addBubble(Role::User, text);
        emit LosCore::LosRouter::instance()._cmd_agent_sendMessage(text, ui -> provider_combo->currentText(), ui->model_combo->currentText());
        ui->input_edit->clear();
    }



    /**
     * @brief onAgentReply
     */
    void LosAgentUi::onAgentReply(const QString &message)
    {
        INF("LosAgentUi", message);
        addBubble(Role::Agent, message);
    }



    /**
     * @brief
     *
     */
    void LosAgentUi::onAgentError(const QString &message)
    {
        ERR("onAgentError", message);
        addBubble(Role::Agent, message);
    }



    /**
     * @brief addBubble
     * - 向消息列表追加一个气泡
     * - User 靠右, Agent 靠左; 配色由 objectName 在 LosAgent_style.h 控制
     *
     * @param role     消息角色
     * @param content  消息内容
     */
    void LosAgentUi::addBubble(Role role, const QString &content)
    {
        const bool isUser    = (role == Role::User);
        const int viewW      = ui->chat_view->viewport()->width();
        const int maxBubbleW = qMax(120, static_cast<int>(viewW * 0.75));
        const int padH       = 6;
        const int padV       = 4;
        const int textW      = maxBubbleW - padH * 2;

        QWidget *bubble = nullptr; // 统一用基类指针, 便于后面放进布局
        int bubbleH     = 0;

        if (isUser)
        {
            // 用户气泡: 纯文本 QLabel (无需 Markdown)
            QLabel *label = new QLabel(content);
            label->setObjectName(QStringLiteral("agentBubbleUser"));
            label->setWordWrap(true);
            label->setTextInteractionFlags(Qt::TextSelectableByMouse);
            label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
            label->setContentsMargins(padH, padV, padH, padV);
            label->setMaximumWidth(maxBubbleW);
            int textH = label->heightForWidth(textW);
            if (textH <= 0)
                textH = label->fontMetrics().height();
            bubbleH = textH + padV * 2;
            bubble  = label;
        }
        else
        {
            // Agent 气泡: QTextBrowser 渲染 Markdown
            QTextBrowser *browser = new QTextBrowser();
            browser->setObjectName(QStringLiteral("agentBubbleAgent"));
            browser->setFrameShape(QFrame::NoFrame);
            browser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            browser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            browser->setOpenExternalLinks(true);
            browser->setTextInteractionFlags(Qt::TextBrowserInteraction);
            browser->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
            browser->setMaximumWidth(maxBubbleW);
            browser->document()->setDocumentMargin(padV);
            browser->document()->setMarkdown(content); // Qt6 原生 Markdown 渲染
            browser->document()->setTextWidth(textW);   // 固定排版宽度后才能算准高度
            const qreal docH = browser->document()->size().height();
            bubbleH          = static_cast<int>(docH) + padV * 2;
            browser->setFixedHeight(bubbleH);
            bubble = browser;
        }

        QWidget *holder  = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout(holder);
        lay->setContentsMargins(8, 4, 8, 4);
        if (isUser)
        {
            lay->addStretch(1);
            lay->addWidget(bubble);
        }
        else
        {
            lay->addWidget(bubble);
            lay->addStretch(1);
        }
        const int itemH       = bubbleH + 8 + 6;
        QListWidgetItem *item = new QListWidgetItem(ui->chat_view);
        item->setSizeHint(QSize(viewW, itemH));
        ui->chat_view->setItemWidget(item, holder);
        ui->chat_view->scrollToBottom();
    }
} /* namespace LosView */
