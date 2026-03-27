#pragma once
#include <QString>

namespace LosStyle {

inline const QString LosIssuesUi_getStyle() {
    return QString(R"(
        QTableWidget {
            background-color: #1E1E1E;
            color: #D4D4D4;
            border: none;
            gridline-color: #333333;
            font-size: 0.95em;
            outline: none;
        }
        QTableWidget::item {
            padding: 4px 8px;
        }
        QTableWidget::item:hover {
            background-color: #2A2D2E;
        }
        QTableWidget::item:selected {
            background-color: #062F4A;
            color: #FFFFFF;
        }
        QHeaderView {
            background-color: #252526;
            border: none;
        }
        QHeaderView::section {
            background-color: #252526;
            color: #A6A6A6;
            padding: 6px 8px;
            border: none;
            border-right: 1px solid #333333;
            border-bottom: 1px solid #333333;
            font-weight: bold;
            font-size: 0.9em;
            text-transform: uppercase;
        }
        QHeaderView::section:last {
            border-right: none;
        }
        QHeaderView::section:hover {
            background-color: #333333;
            color: #FFFFFF;
        }
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: #454545; 
            border-radius: 6px;
            min-height: 30px;
            margin: 2px; 
        }
        QScrollBar::handle:vertical:hover {
            background-color: #555555;
        }
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background-color: #454545;
            border-radius: 6px;
            min-width: 30px;
            margin: 2px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #555555;
        }
        QScrollBar::add-line, QScrollBar::sub-line, QScrollBar::add-page, QScrollBar::sub-page {
            height: 0px; 
            width: 0px;
            background-color: transparent; 
        }
    )");
}

} // namespace LosStyle
