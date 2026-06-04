// Copyright (c) 2026 LosAngelous (shengjie.lin)



#pragma once


namespace LosCommon
{

    namespace LosEditorTableUi_Constants
    {
        inline constexpr const char *ACTION_PIN             = "pin";
        inline constexpr const char *ACTION_COPY_LOCAL_PATH = "copy local path";
        inline constexpr const char *CLOSE_BTN_STYLE = R"(
            QToolButton#tab_close_btn {
                background: transparent;
                color: #6272a4;
                border: none;
                border-radius: 4px;
                padding: 0px;
                margin: 0px;
                font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
                font-size: 16px;
                font-weight: bold;
            }
            QToolButton#tab_close_btn:hover {
                background-color: #ff5555;
                color: #f8f8f2;
            }
            QToolButton#tab_close_btn:pressed {
                background-color: #c13e3e;
                color: #f8f8f2;
            }
        )";
    }; // namespace LosEditorTableUi_Constants
} // namespace LosCommon