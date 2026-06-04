// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{

    inline const QString LosIssuesUi_getStyle()
    {
        return QString(R"(
        QTableWidget {
            background-color: #282a36;
            color: #f8f8f2;
            border: none;
            gridline-color: #44475a;
            font-size: 13px;
            outline: none;
        }
        QTableWidget::item {
            padding: 4px 8px;
        }
        QTableWidget::item:hover {
            background-color: #44475a;
        }
        QTableWidget::item:selected {
            background-color: #44475a;
            color: #bd93f9;
        }
        QHeaderView {
            background-color: #21222c;
            border: none;
        }
        QHeaderView::section {
            background-color: #21222c;
            color: #6272a4;
            padding: 6px 8px;
            border: none;
            border-right: 1px solid #44475a;
            border-bottom: 1px solid #44475a;
            font-weight: bold;
            font-size: 12px;
            text-transform: uppercase;
        }
        QHeaderView::section:last {
            border-right: none;
        }
        QHeaderView::section:hover {
            background-color: #44475a;
            color: #bd93f9;
        }
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: #6272a4;
            border-radius: 6px;
            min-height: 30px;
            margin: 2px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #bd93f9;
        }
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background-color: #6272a4;
            border-radius: 6px;
            min-width: 30px;
            margin: 2px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #bd93f9;
        }
        QScrollBar::add-line, QScrollBar::sub-line, QScrollBar::add-page, QScrollBar::sub-page {
            height: 0px; 
            width: 0px;
            background-color: transparent; 
        }
    )");
    }

} /* namespace LosStyle */
