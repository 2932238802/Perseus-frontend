
#include "LosTerminalUi.h"

namespace LosView
{
    LosTerminalUi::LosTerminalUi(QWidget *parent) : QWidget(parent)
    {
        initStyle();
    }
    LosTerminalUi::~LosTerminalUi() {}



    /**
     * @brief initStyle
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
        L_webView->setUrl(QUrl("qrc:/web/resources/web/terminal.html"));
        layout->addWidget(L_webView);
    }



    void LosTerminalUi::initConnect() {}
} /* namespace LosView */
