// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosTerminalUi.h"

#include <QColor>
#include <QUrl>
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

        L_webView->page()->setBackgroundColor(QColor("#282a36"));
        L_webView->setUrl(QUrl("qrc:/web/web/terminal.html"));
        layout->addWidget(L_webView);
    }



    /**
     * @brief initConnect
     *
     */
    void LosTerminalUi::initConnect() {}
} /* namespace LosView */
