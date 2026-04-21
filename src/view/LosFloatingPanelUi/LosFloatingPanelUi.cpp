
#include "LosFloatingPanelUi.h"

namespace LosView
{
    LosFloatingPanelUi::LosFloatingPanelUi(QWidget *contentWidget, bool auto_close, QWidget *parent) : QDialog(parent)
    {

        if (auto_close)
        {
            setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        }
        else
        {
            setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        }
        setAttribute(Qt::WA_TranslucentBackground);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(10, 10, 10, 10);

        L_bgWidget = new QWidget(this);
        L_bgWidget->setStyleSheet(R"(
        QWidget { 
            background-color: #181825; 
            border: 1px solid #45475a; 
            border-radius: 6px; 
            color: #cdd6f4; 
            font-family: 'Microsoft YaHei', sans-serif;
        }
    )");

        /*
         * 布局
         */
        QVBoxLayout *bgLayout = new QVBoxLayout(L_bgWidget);
        bgLayout->setContentsMargins(8, 8, 8, 8);

        /*
         * 设置样式
         */
        contentWidget->setObjectName("LosContentWrapper");
        contentWidget->setStyleSheet("QWidget#LosContentWrapper { background-color: transparent; border: none; }");

        /*
         * contentWidget
         * 这个东西 加入 到 布局
         */
        bgLayout->addWidget(contentWidget);
        mainLayout->addWidget(L_bgWidget);
    }



    void LosFloatingPanelUi::initStyle() {}



    void LosFloatingPanelUi::showAtPosition(QWidget *parent, LosCommon::LosFloatingPanelUi_Constants::PositionMode mode)
    {
        if (!parent)
            return;
        this->adjustSize();

        QPoint targetPos;
        QRect targetRect     = parent->rect();
        QPoint globalTopLeft = parent->mapToGlobal(targetRect.topLeft());

        switch (mode)
        {
        case LosCommon::LosFloatingPanelUi_Constants::PositionMode::Center:
            targetPos =
                globalTopLeft + QPoint((targetRect.width() - width()) / 2, (targetRect.height() - height()) / 2 - 40);
            break;
        case LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopRight:
            targetPos = globalTopLeft + QPoint(targetRect.width() - width() - 20, 20);
            break;
        case LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopCenter:
            targetPos = globalTopLeft + QPoint((targetRect.width() - width()) / 2, 10);
            break;
        }
        this->move(targetPos);
    }



    /*
     * - 按键 按 esc 自动退出
     */
    void LosFloatingPanelUi::keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            this->reject();
        }
        QDialog::keyPressEvent(event);
    }



} /* namespace LosView */