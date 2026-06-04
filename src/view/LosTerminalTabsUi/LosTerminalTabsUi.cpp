// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosTerminalTabsUi.h"
#include "common/util/GetBtn.h"
#include <qpushbutton.h>
#include <qtabbar.h>


namespace LosView
{
    LosTerminalTabsUi::LosTerminalTabsUi(QWidget *parent) : QTabWidget(parent)
    {
        initStyle();
    }



    /**
     * @brief initStyle
     * setTabsClosable(false); 关闭自动绘制的叉叉
     *
     *
     * 初始化 样式
     */
    void LosTerminalTabsUi::initStyle()
    {
        setTabsClosable(false);
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



    /**
     * @brief initConnect
     * 初始化链接
     */
    void LosTerminalTabsUi::initConnect() {}



    /**
     * @brief addNewTerminal
     * 新增 终端
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
        QPushButton *closeBtn = LosCommon::GetBtn<QPushButton>("x", LosCommon::LosTerminal_Constants::CLOSEBTN_STYLE, QSize(16, 16), newOne);
        int newIndex          = addTab(newOne, QString("sh - %1").arg(newId));
        connect(closeBtn, &QPushButton::clicked, this,
                [this, newOne]()
                {
                    int idx = indexOf(newOne);
                    if (idx < 0)
                        return;
                    bool ok;
                    int id = newOne->property("terminal_id").toInt(&ok);
                    if (ok)
                        L_usedTerminalIds.remove(id);
                    removeTab(idx);
                    newOne->deleteLater();
                    if (count() == 0)
                    {
                        addNewTerminal();
                    }
                });
        tabBar()->setTabButton(newIndex, QTabBar::RightSide, closeBtn);
        setCurrentIndex(newIndex);
    }
} /* namespace LosView */