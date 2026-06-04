// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{

    inline const QString LosCommandUi_style()
    {
        return QString(R"(
            #PaletteContainer {
                background-color: #21222c;
                border: 1px solid #bd93f9;
                border-radius: 8px;
            }

            QLineEdit {
                background-color: transparent;
                color: #f8f8f2;
                border: none;
                border-bottom: 1px solid #44475a;
                padding: 12px 15px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 14px;
                selection-background-color: #bd93f9;
                selection-color: #282a36;
            }

            QLineEdit:focus {
                border-bottom: 1px solid #bd93f9;
            }

            QListWidget {
                background-color: transparent;
                color: #f8f8f2;
                border: none;
                outline: none;
                padding: 6px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 13px;
            }

            QListWidget::item {
                padding: 8px 10px;
                border-radius: 4px;
                margin: 2px 4px;
            }

            QListWidget::item:hover {
                background-color: #44475a;
            }

            QListWidget::item:selected {
                background-color: #44475a;
                color: #bd93f9;
                border-left: 3px solid #bd93f9;
                border-radius: 0px;
            }
        )");
    }

} /* namespace LosStyle */
