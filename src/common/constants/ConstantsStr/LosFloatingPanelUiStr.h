// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosCommon
{
    namespace LosFloatingPanelUi_Constants
    {
        namespace LosGotoLinePopupUi_Constants
        {
            inline constexpr const char *GOTOLINE_LABEL_CONTENT    = "Go to line:";
            inline constexpr const char *GOTOLINE_LABEL_STYLESHEET = R"(
                                                                        color: #a6adc8; 
                                                                        font-size: 1em;
                                                                    )";

            inline constexpr const char *GOTOLINE_LINEEDIT_STYLESHEET = R"(
                                                                            QLineEdit { 
                                                                                background-color: #a1a3c1; 
                                                                                border: 1px solid #45475a; 
                                                                                border-radius: 4px; 
                                                                                padding: 4px 8px; 
                                                                                color: #ffffff; 
                                                                                font-family: 'JetBrains Mono', monospace;
                                                                                font-size: 1.1em;
                                                                            }
                                                                            QLineEdit:focus { 
                                                                                border: 1px solid #89b4fa; 
                                                                                background-color: #45475a; 
                                                                            }
                                                                        )";
        } /* namespace LosGotoLinePopupUi_Constants */



        namespace LosFindPopupUi_Constants
        {
            inline constexpr const char *FIND_LABEL_CONTENT    = "Find:";
            inline constexpr const char *REPLACE_LABEL_CONTENT = "Replace:";

            inline constexpr const char *FIND_LABEL_STYLESHEET = R"(
                                                                        color: #a6adc8; 
                                                                        font-size: 1em;
                                                                    )";

            inline constexpr const char *FIND_MATCH_LABEL_STYLESHEET = R"(
                                                                        color: #a6adc8; 
                                                                        font-size: 0.9em;
                                                                        background-color: #313244;
                                                                        border-radius: 3px;
                                                                        padding: 2px 6px;
                                                                    )";


            inline constexpr const char *FIND_LINEEDIT_STYLESHEET    = R"(
                                                                            QLineEdit { 
                                                                                background-color: #a1a3c1; 
                                                                                border: 1px solid #45475a; 
                                                                                border-radius: 4px; 
                                                                                padding: 4px 8px; 
                                                                                color: #ffffff; 
                                                                                font-family: 'JetBrains Mono', monospace;
                                                                                font-size: 1.1em;
                                                                            }
                                                                            QLineEdit:focus { 
                                                                                border: 1px solid #89b4fa; 
                                                                                background-color: #45475a; 
                                                                            }
                                                                        )";
            inline constexpr const char *REPLACE_LINEEDIT_STYLESHEET = FIND_LINEEDIT_STYLESHEET;

            inline constexpr const char *FIND_BUTTON_STYLESHEET = R"(
                                                                    QPushButton {
                                                                        background-color: #313244;
                                                                        color: #cdd6f4;
                                                                        border: 1px solid #45475a;
                                                                        border-radius: 4px;
                                                                        padding: 4px 10px;
                                                                        font-size: 0.9em;
                                                                    }
                                                                    QPushButton:hover {
                                                                        background-color: #45475a;
                                                                    }
                                                                    QPushButton:pressed {
                                                                        background-color: #585b70;
                                                                    }
                                                                )";

            inline constexpr const char *FIND_CHECKBOX_STYLESHEET = R"(
                                                                    QCheckBox {
                                                                        background-color: #313244;
                                                                        color: #a6adc8;
                                                                        border: 1px solid #45475a;
                                                                        border-radius: 13px;
                                                                        padding: 4px 16px;
                                                                        font-size: 0.9em;
                                                                        spacing: 8px;
                                                                    }
                                                                    QCheckBox:hover {
                                                                        background-color: #3b3f58;
                                                                        border-color: #89b4fa;
                                                                        color: #cdd6f4;
                                                                    }
                                                                    QCheckBox:checked {
                                                                        background-color: #89b4fa;
                                                                        border-color: #89b4fa;
                                                                        color: #1e1e2e;
                                                                        font-weight: 600;
                                                                    }
                                                                    QCheckBox:checked:hover {
                                                                        background-color: #a0bffc;
                                                                        border-color: #a0bffc;
                                                                    }
                                                                    QCheckBox::indicator {
                                                                        width: 0px;
                                                                        height: 0px;
                                                                        border: none;
                                                                    }
                                                                )";

        } // namespace LosFindPopupUi_Constants
    } /* namespace LosFloatingPanelUi_Constants */
} // namespace LosCommon