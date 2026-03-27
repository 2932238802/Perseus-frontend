#pragma once
#include <QString>

namespace LosStyle {

inline const QString LosCompleterUi_getStyle() {
    return QString(R"(
        QListView {
            background-color: #1E1E1E;
            color: #D4D4D4;
            border: 1px solid #454545;
            border-radius: 4px;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 1.1em;
            padding: 4px;
            outline: none;
        }
        QListView::item {
            padding: 6px 8px;
            border-radius: 4px;
            margin: 2px 0px;
        }
        QListView::item:hover {
            background-color: #2D2D30;
        }
        QListView::item:selected {
            background-color: #062F4A;
            color: #FFFFFF;
            border: none;
            font-weight: bold;
        }
        QScrollBar:vertical {
            background-color: transparent;
            width: 10px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: #454545; 
            border-radius: 5px;
            min-height: 20px;
            margin: 2px; 
        }
        QScrollBar::handle:vertical:hover {
            background-color: #555555;
        }
        QScrollBar::add-line, QScrollBar::sub-line, QScrollBar::add-page, QScrollBar::sub-page {
            height: 0px; 
            background-color: transparent; 
        }
    )");
}

} // namespace LosStyle
