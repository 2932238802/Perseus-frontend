// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{
    inline const QString LosCommandArgsUi_styleTemplate()
    {
        return QString(R"(
            QDialog {
                background-color: @background@;
                border: 1px solid @primary@;
                border-radius: 6px;
            }
            QLabel {
                color: @foreground@;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }
            QLabel#label_title {
                color: @primary@;
                font-size: 16px;
                font-weight: bold;
                border-bottom: 1px solid @selection@;
                padding-bottom: 8px;
            }
            QLineEdit {
                background-color: @panelBg@;
                color: @foreground@;
                border: 1px solid @selection@;
                border-radius: 3px;
                padding: 6px 10px;
                font-family: "Consolas", monospace;
                font-size: 13px;
                selection-background-color: @primary@;
                selection-color: @background@;
            }
            QLineEdit:focus {
                border: 1px solid @primary@;
            }
            QScrollArea {
                background-color: transparent;
            }
            QWidget#scrollAreaWidgetContents {
                background-color: transparent;
            }
            QPushButton {
                background-color: @selection@;
                color: @foreground@;
                border: none;
                border-radius: 3px;
                padding: 6px 18px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }
            QPushButton:hover {
                background-color: @muted@;
            }
            QPushButton:pressed {
                background-color: @primary@;
                color: @background@;
            }
            QPushButton#btn_run {
                background-color: @primary@;
                color: @background@;
                font-weight: bold;
            }
            QPushButton#btn_run:hover {
                background-color: @primaryHover@;
            }
            QPushButton#btn_run:pressed {
                background-color: @primaryPressed@;
                color: @foreground@;
            }
        )");
    }
} // namespace LosStyle
