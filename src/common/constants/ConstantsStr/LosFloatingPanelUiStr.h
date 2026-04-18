#include <QString>

namespace LosCommon
{
    /*
     * 悬浮面板与弹窗的 UI 样式常量
     */
    namespace LosFloatingPanelUi_Constants
    {
        namespace LosGotoLinePopupUi_Constants
        {
            inline constexpr const char *GOTOLINE_LABEL_CONTENT    = "Go to line:";
            inline constexpr const char *GOTOLINE_LABEL_STYLESHEET = R"(
                                                                        color: #a6adc8; 
                                                                        font-size: 13px;
                                                                    )";

            inline constexpr const char *GOTOLINE_LINEEDIT_STYLESHEET = R"(
                                                                            QLineEdit { 
                                                                                background-color: #a1a3c1; 
                                                                                border: 1px solid #45475a; 
                                                                                border-radius: 4px; 
                                                                                padding: 4px 8px; 
                                                                                color: #ffffff; 
                                                                                font-family: 'JetBrains Mono', monospace;
                                                                                font-size: 14px;
                                                                            }
                                                                            QLineEdit:focus { 
                                                                                border: 1px solid #89b4fa; 
                                                                                background-color: #45475a; 
                                                                            }
                                                                        )";
        } /* namespace LosGotoLinePopupUi_Constants */
    } /* namespace LosFloatingPanelUi_Constants */
} // namespace LosCommon