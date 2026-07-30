// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{
    inline const QString LosCompleterUi_styleTemplate()
    {
        return QString(R"(
        QListView {
            background-color: @background@;
            color: @foreground@;
            border: 1px solid @primary@;
            border-radius: 6px;
            font-family: "JetBrains Mono", "Consolas", "Noto Sans Mono CJK SC", "Menlo", monospace;
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
            background-color: @selection@;
            color: @foreground@;
        }
        QListView::item:selected {
            background-color: @primary@;
            color: @background@;
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
            background-color: @muted@;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: @primary@;
        }
        QScrollBar::add-line, QScrollBar::sub-line,
        QScrollBar::add-page, QScrollBar::sub-page {
            height: 0px;
            background-color: transparent;
        }
    )");
    }

} /* namespace LosStyle */
