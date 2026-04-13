#pragma once
#include <QString>

namespace LosStyle
{
    inline const QString LosCommandArgsUi_style()
    {
        return QString(R"(
            QDialog {
                background-color: #1e1e1e;
                border: 1px solid #454545;
                border-radius: 6px;
            }
            QLabel {
                color: #cccccc;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }
            QLabel#label_title {
                color: #ffffff;
                font-size: 16px;
                font-weight: bold;
                border-bottom: 1px solid #333333;
                padding-bottom: 8px;
            }
            QLineEdit {
                background-color: #252526;
                color: #ffffff;
                border: 1px solid #333333;
                border-radius: 3px;
                padding: 6px 10px;
                font-family: "Consolas", monospace;
                font-size: 13px;
            }
            QLineEdit:focus {
                border: 1px solid #007acc;
            }
            QScrollArea {
                background-color: transparent;
            }
            QWidget#scrollAreaWidgetContents {
                background-color: transparent;
            }
            QPushButton {
                background-color: #333333;
                color: #ffffff;
                border: none;
                border-radius: 3px;
                padding: 6px 18px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }
            QPushButton:hover {
                background-color: #454545;
            }
            QPushButton:pressed {
                background-color: #2a2d2e;
            }
            QPushButton#btn_run {
                background-color: #007acc;
                font-weight: bold;
            }
            QPushButton#btn_run:hover {
                background-color: #005f9e;
            }
            QPushButton#btn_run:pressed {
                background-color: #004a7c;
            }
        )");
    }
} // namespace LosStyle
