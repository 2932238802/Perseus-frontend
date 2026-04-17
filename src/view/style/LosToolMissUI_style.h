#include <QString>

namespace LosStyle
{
    inline QString LosToolMissUI_getStyle()
    {
        return QString(R"(
            QDialog {
                background-color: #252526;
                color: #cccccc;
            }
            
            /* 普通文本标签 */
            QLabel {
                color: #cccccc;
            }
            
            QPushButton {
                background-color: #333333;
                border: 1px solid #3c3c3c;
                padding: 6px 12px;
                border-radius: 4px;
                color: #cccccc;
            }
            QPushButton:hover {
                background-color: #404040;
                color: #ffffff;
            }
            QPushButton:pressed {
                background-color: #2d2d2d;
            }
            
            QPushButton#primaryBtn {
                background-color: #0e639c;  /* VS Code 经典的强调蓝 */
                border: 1px solid #0e639c;
                padding: 8px 16px;
                font-weight: bold;
                color: #ffffff;
            }
            QPushButton#primaryBtn:hover {
                background-color: #1177bb;  /* 悬停时变亮 */
            }
            QPushButton#primaryBtn:pressed {
                background-color: #0d5484;  /* 按下时变暗 */
            }
        )");
    };
} /* namespace LosStyle */
