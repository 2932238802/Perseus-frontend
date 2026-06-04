// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{
    inline QString LosToolMissUI_styleTemplate()
    {
        return QString(R"(
            QDialog {
                background-color: @background@;
                color: @foreground@;
            }

            QLabel {
                color: @foreground@;
            }

            QPushButton {
                background-color: @selection@;
                border: 1px solid @selection@;
                padding: 6px 12px;
                border-radius: 4px;
                color: @foreground@;
            }
            QPushButton:hover {
                background-color: @muted@;
                border: 1px solid @primary@;
                color: @highlightFg@;
            }
            QPushButton:pressed {
                background-color: @primary@;
                color: @background@;
            }

            QPushButton#primaryBtn {
                background-color: @primary@;
                border: 1px solid @primary@;
                padding: 8px 16px;
                font-weight: bold;
                color: @background@;
            }
            QPushButton#primaryBtn:hover {
                background-color: @primaryHover@;
                border: 1px solid @primaryHover@;
            }
            QPushButton#primaryBtn:pressed {
                background-color: @primaryPressed@;
                border: 1px solid @primaryPressed@;
                color: @foreground@;
            }
        )");
    };
} /* namespace LosStyle */
