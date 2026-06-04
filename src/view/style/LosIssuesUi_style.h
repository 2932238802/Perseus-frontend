// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{

    inline const QString LosIssuesUi_styleTemplate()
    {
        return QString(R"(
        QTableWidget {
            background-color: @background@;
            color: @foreground@;
            border: none;
            gridline-color: @selection@;
            font-size: 13px;
            outline: none;
        }
        QTableWidget::item {
            padding: 4px 8px;
        }
        QTableWidget::item:hover {
            background-color: @selection@;
        }
        QTableWidget::item:selected {
            background-color: @selection@;
            color: @primary@;
        }
        QHeaderView {
            background-color: @panelBg@;
            border: none;
        }
        QHeaderView::section {
            background-color: @panelBg@;
            color: @muted@;
            padding: 6px 8px;
            border: none;
            border-right: 1px solid @selection@;
            border-bottom: 1px solid @selection@;
            font-weight: bold;
            font-size: 12px;
            text-transform: uppercase;
        }
        QHeaderView::section:last {
            border-right: none;
        }
        QHeaderView::section:hover {
            background-color: @selection@;
            color: @primary@;
        }
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: @muted@;
            border-radius: 6px;
            min-height: 30px;
            margin: 2px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: @primary@;
        }
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background-color: @muted@;
            border-radius: 6px;
            min-width: 30px;
            margin: 2px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: @primary@;
        }
        QScrollBar::add-line, QScrollBar::sub-line, QScrollBar::add-page, QScrollBar::sub-page {
            height: 0px; 
            width: 0px;
            background-color: transparent; 
        }
    )");
    }

} /* namespace LosStyle */
