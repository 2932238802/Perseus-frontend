// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosPanelManager.h"
#include "common/constants/ConstantsStr/LosPanelManagerStr.h"
#include "common/util/GetBtn.h"
#include <QTabBar>
#include <qobject.h>


namespace LosCore
{
    LosPanelManager::LosPanelManager(QTabWidget *tab, QObject *parent) : QObject(parent), L_tab(tab) {}


    /**
     * @brief registerPanel
     * 存在ui里面的 然后 登记在 管理器里面
     * @param id
     * @param widget
     * @param displayName
     * @param order
     */
    void LosPanelManager::registerPanel(const QString &id, QWidget *widget, const QString &displayName,

                                        int order

    )
    {
        if (!L_tab || !widget || id.isEmpty())
        {
            return;
        }

        if (LOS_panels.contains(id))
        {
            return;
        }

        LosCommon::LosPanelManager_Constants::Panel panel;
        int index           = L_tab->indexOf(widget);
        panel.L_id          = id;
        panel.L_widget      = widget;
        panel.L_displayName = displayName;
        panel.visible       = (index >= 0);
        panel.order         = order;
        panel.L_viewAction  = nullptr;
        if (index >= 0)
        {
            L_tab->setTabText(index, displayName);
        }
        LOS_panels.insert(id, panel);
    }



    /**
     * @brief addCloseBtn
     * 增加关闭 按钮
     */
    void LosPanelManager::addCloseBtn()
    {
        if (!L_tab || LOS_panels.isEmpty())
            return;
        for (auto value : LOS_panels)
        {
            auto widget = value.L_widget;
            if (!widget)
                continue;
            int index = L_tab->indexOf(widget);

            auto *btn = LosCommon::GetBtn<QToolButton>("x", LosCommon::LosPanelManager_Constants::CLOSE_BTN_STYLE,
                                                       {16, 16}, L_tab->tabBar());
            initCloseBtnConnect(btn, widget);
            L_tab->tabBar()->setTabButton(index, QTabBar::RightSide, btn);
        }
    }



    /**
     * @brief hidePanel
     * 隐藏窗口
     */
    void LosPanelManager::hidePanel(const QString &panel_id)
    {
        if (!L_tab || !panel_id.isEmpty())
            return;
        auto findPanel = LOS_panels.find(panel_id);
        if (findPanel == LOS_panels.end())
        {
            return;
        }
        auto &panel = findPanel.value();
        if (!panel.L_widget)
            return;
        int index = L_tab->indexOf(panel.L_widget);
        if (index < 0)
        {
            panel.visible = false;
            return;
        }
        L_tab->removeTab(index);
        panel.L_widget->hide();
        panel.visible = false;
        if (panel.L_viewAction)
        {
            // 这里其实是 RALL 思想
            // 因为 屏蔽了这里 才能在下一行 setChecked的时候 不会触发信号槽
            QSignalBlocker block(panel.L_viewAction);
            panel.L_viewAction->setChecked(false);
        }
        if (L_tab->count() == 0)
        {
            L_tab->hide();
        }
    }



    /**
     * @brief initCloseBtnConnect
     * 增加链接
     *
     * @param btn
     */
    void LosPanelManager::initCloseBtnConnect(QAbstractButton *btn, QWidget *parent)
    {
        if (!btn || !parent)
            return;
        connect(btn, &QAbstractButton::clicked, this,
                [this, parent]()
                {
                    for (auto it = LOS_panels.begin(); it != LOS_panels.end(); ++it)
                    {
                        if (it.value().L_widget == parent)
                        {
                            hidePanel(it.key());
                            return;
                        }
                    }
                });
    }
} // namespace LosCore