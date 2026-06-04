// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{
    inline const QString LosCompleterUi_getStyle()
    {
        return QString(R"(
        QListView {
            background-color: #282a36;
            color: #f8f8f2;
            border: 1px solid #bd93f9;
            border-radius: 6px;
            font-family: "JetBrains Mono", "Consolas", "Menlo", monospace;
            font-size: 13px;
            padding: 4px;
            outline: none;
            show-decoration-selected: 1;
        }
        QListView::item {
            padding: 5px 10px;
            border-radius: 4px;
            margin: 1px 2px;
            border: none;
        }
        QListView::item:hover {
            background-color: #44475a;
            color: #f8f8f2;
        }
        QListView::item:selected {
            background-color: #bd93f9;
            color: #282a36;
            border: none;
            font-weight: bold;
        }
        QScrollBar:vertical {
            background-color: transparent;
            width: 8px;
            border: none;
            margin: 2px;
        }
        QScrollBar::handle:vertical {
            background-color: #6272a4;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #bd93f9;
        }
        QScrollBar::add-line, QScrollBar::sub-line,
        QScrollBar::add-page, QScrollBar::sub-page {
            height: 0px;
            background-color: transparent;
        }
    )");
    }

} /* namespace LosStyle */
