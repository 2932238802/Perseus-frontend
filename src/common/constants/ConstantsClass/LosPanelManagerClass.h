// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>
#include <qaction.h>
#include <qwidget.h>

namespace LosCommon
{
    namespace LosPanelManager_Constants
    {
        struct Panel
        {
            QString L_id;
            QString L_displayName;
            QWidget *L_widget = nullptr;
            int order         = 0;
            bool visible      = true;
            QAction *L_viewAction;
        };
    } // namespace LosPanelManager_Constants
} // namespace LosCommon