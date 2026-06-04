// Copyright (c) 2026 LosAngelous (shengjie.lin)


#pragma once
#include <QString>
namespace LosCommon
{
    namespace LosPanelManager_Constants
    {
        inline constexpr const char* OUTPUT = "panel.output";
        inline constexpr const char* ISSUSES = "panel.issuses";
        inline constexpr const char* TERMINAL = "panel.terminal";

        inline const QString CLOSE_BTN_STYLE = R"(
            QToolButton {
            background: transparent;
            color: #8a8fa3;
            border: none;
            padding: 0px;
            font-size: 13px;
            font-weight: bold;
            }
            QToolButton:hover {
                color: #ffffff;
                background-color: #d65d5d;
                border-radius: 3px;
            }
            QToolButton:pressed {
                background-color: #b84a4a;
            }
            )";
    }
} // namespace LosCommon