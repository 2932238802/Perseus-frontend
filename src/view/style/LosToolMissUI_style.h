// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include <QString>

namespace LosStyle
{
    inline QString LosToolMissUI_getStyle()
    {
        return QString(R"(
            QDialog {
                background-color: #282a36;
                color: #f8f8f2;
            }

            QLabel {
                color: #f8f8f2;
            }

            QPushButton {
                background-color: #44475a;
                border: 1px solid #44475a;
                padding: 6px 12px;
                border-radius: 4px;
                color: #f8f8f2;
            }
            QPushButton:hover {
                background-color: #6272a4;
                border: 1px solid #bd93f9;
                color: #ffffff;
            }
            QPushButton:pressed {
                background-color: #bd93f9;
                color: #282a36;
            }

            QPushButton#primaryBtn {
                background-color: #bd93f9;
                border: 1px solid #bd93f9;
                padding: 8px 16px;
                font-weight: bold;
                color: #282a36;
            }
            QPushButton#primaryBtn:hover {
                background-color: #caa9fa;
                border: 1px solid #caa9fa;
            }
            QPushButton#primaryBtn:pressed {
                background-color: #9580d4;
                border: 1px solid #9580d4;
                color: #f8f8f2;
            }
        )");
    };
} /* namespace LosStyle */
