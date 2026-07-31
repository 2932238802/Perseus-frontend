// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosTerminalUi.h"

#include "core/LosRouter/LosRouter.h"
#include "core/LosTheme/LosThemeManager.h"

#include <QColor>
#include <QUrl>
#include <QUrlQuery>
#include <QVBoxLayout>

namespace LosView
{
    /**
     * @brief Construct a new Los Terminal Ui:: Los Terminal Ui object
     *
     * @param parent
     */
    LosTerminalUi::LosTerminalUi(QWidget *parent) : QWidget(parent)
    {
        initStyle();
        initConnect();
    }
    LosTerminalUi::~LosTerminalUi() {}



    /**
     * @brief initStyle 初始化样式
     *
     */
    void LosTerminalUi::initStyle()
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        L_webView = new QWebEngineView(this);
        L_channel = new QWebChannel(this);
        L_backend = new LosCore::LosTerminal(this);

        L_channel->registerObject("backend", L_backend);
        L_webView->page()->setWebChannel(L_channel);

        const QColor bg = themeBackground();
        L_webView->page()->setBackgroundColor(bg);
        QUrl url(QStringLiteral("qrc:/web/web/terminal.html"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("bg"), bg.name());
        query.addQueryItem(QStringLiteral("fg"), themeForeground());
        url.setQuery(query);
        L_webView->setUrl(url);
        layout->addWidget(L_webView);
    }



    /**
     * @brief initConnect
     *
     */
    void LosTerminalUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this,
                [this](const QString &)
                {
                    const QColor bg = themeBackground();
                    L_webView->page()->setBackgroundColor(bg);
                    L_webView->page()->runJavaScript(QStringLiteral("applyTheme('%1','%2')").arg(bg.name(), themeForeground()));
                });
    }



    /**
     * @brief themeBackground 当前主题的背景色, 与 terminal.html 保持同步
     *
     * @return QColor
     */
    QColor LosTerminalUi::themeBackground() const
    {
        const auto tokens = LosCore::LosThemeManager::instance().uiTokens(LosCore::LosThemeManager::instance().currentTheme());
        QColor color(tokens.value(QStringLiteral("background"), QStringLiteral("#1e1e1e")));
        if (!color.isValid())
        {
            color = QColor(QStringLiteral("#1e1e1e"));
        }
        return color;
    }



    /**
     * @brief themeForeground 当前主题的前景色, 与 terminal.html 保持同步
     *
     * @return QString
     */
    QString LosTerminalUi::themeForeground() const
    {
        const auto tokens = LosCore::LosThemeManager::instance().uiTokens(LosCore::LosThemeManager::instance().currentTheme());
        return tokens.value(QStringLiteral("foreground"), QStringLiteral("#cccccc"));
    }
} /* namespace LosView */
