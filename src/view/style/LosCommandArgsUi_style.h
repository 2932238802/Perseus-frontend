// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{
    inline const QString LosCommandArgsUi_style()
    {
        return QString(R"(
            QDialog {
                background-color: #282a36;
                border: 1px solid #bd93f9;
                border-radius: 6px;
            }
            QLabel {
                color: #f8f8f2;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }
            QLabel#label_title {
                color: #bd93f9;
                font-size: 16px;
                font-weight: bold;
                border-bottom: 1px solid #44475a;
                padding-bottom: 8px;
            }
            QLineEdit {
                background-color: #21222c;
                color: #f8f8f2;
                border: 1px solid #44475a;
                border-radius: 3px;
                padding: 6px 10px;
                font-family: "Consolas", monospace;
                font-size: 13px;
                selection-background-color: #bd93f9;
                selection-color: #282a36;
            }
            QLineEdit:focus {
                border: 1px solid #bd93f9;
            }
            QScrollArea {
                background-color: transparent;
            }
            QWidget#scrollAreaWidgetContents {
                background-color: transparent;
            }
            QPushButton {
                background-color: #44475a;
                color: #f8f8f2;
                border: none;
                border-radius: 3px;
                padding: 6px 18px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }
            QPushButton:hover {
                background-color: #6272a4;
            }
            QPushButton:pressed {
                background-color: #bd93f9;
                color: #282a36;
            }
            QPushButton#btn_run {
                background-color: #bd93f9;
                color: #282a36;
                font-weight: bold;
            }
            QPushButton#btn_run:hover {
                background-color: #caa9fa;
            }
            QPushButton#btn_run:pressed {
                background-color: #9580d4;
                color: #f8f8f2;
            }
        )");
    }
} // namespace LosStyle
