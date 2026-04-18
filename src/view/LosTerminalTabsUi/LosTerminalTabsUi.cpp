#include "LosTerminalTabsUi.h"


namespace LosView
{

    LosTerminalTabsUi::LosTerminalTabsUi(QWidget *parent) : QTabWidget(parent)
    {
        initStyle();
    }



    /*
     * 初始化 样式
     */
    void LosTerminalTabsUi::initStyle()
    {
        setTabsClosable(true);
        setDocumentMode(true);
        clear();

        QToolButton *addBtn = new QToolButton(this);
        addBtn->setText("+ add new");
        addBtn->setCursor(Qt::PointingHandCursor);
        addBtn->setStyleSheet(R"(
        QToolButton { 
            background-color: transparent;
            color: #888888; 
            border: none; 
            padding: 6px 12px; 
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 12px;
            font-weight: bold;
        }
        QToolButton:hover { 
            background-color: #2a2d2e;
            color: #ffffff; 
        }
        QToolButton:pressed {
            background-color: #37373d;
        }
    )");
        setCornerWidget(addBtn, Qt::TopRightCorner);

        connect(addBtn, &QToolButton::clicked, this, &LosTerminalTabsUi::addNewTerminal);
        connect(this, &QTabWidget::tabCloseRequested, this,
                [this](int index)
                {
                    QWidget *w = widget(index);
                    bool ok;
                    int id = w->property("terminal_id").toInt(&ok);
                    if (ok)
                    {
                        L_usedTerminalIds.remove(id);
                    }
                    removeTab(index);
                    w->deleteLater();
                });
        addNewTerminal();
    }



    /*
     * 增加 终端
     */
    void LosTerminalTabsUi::addNewTerminal()
    {
        LosView::LosTerminalUi *newOne = new LosView::LosTerminalUi(this);
        int newId                      = 1;
        while (L_usedTerminalIds.contains(newId))
        {
            newId++;
        }
        L_usedTerminalIds.insert(newId);
        newOne->setProperty("terminal_id", newId);
        int newIndex = addTab(newOne, QString("sh - %1").arg(newId));
        setCurrentIndex(newIndex);
    }

} /* namespace LosView */