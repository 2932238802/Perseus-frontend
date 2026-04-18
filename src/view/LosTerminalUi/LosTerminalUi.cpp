
#include "LosTerminalUi.h"

namespace LosView
{
    LosTerminalUi::LosTerminalUi(QWidget *parent) : QWidget(parent)
    {
        initStyle();
    }
    LosTerminalUi::~LosTerminalUi() {}



    /*
     * - 初始化样式
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

        /*
         * 设置背景 颜色
         */
        L_webView->page()->setBackgroundColor(QColor("#1e1e1e"));
        L_webView->setUrl(QUrl("qrc:/web/resources/web/terminal.html"));
        layout->addWidget(L_webView);
    }



    /*
     * 超级试驾；哦按空耳
     */
    void LosTerminalUi::initConnect() {}


} /* namespace LosView */
