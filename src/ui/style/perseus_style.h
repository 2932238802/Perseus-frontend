#pragma once
#include <QString>

namespace LosStyle {

inline const QString perseus_getStyle() {
    return QString(R"(

        QMainWindow {
            background-color: #1e1e2e;
        }

        #toolbar_widget {
            background-color: #181825;
            border-bottom: 1px solid #313244;
        }

        #icon_btn {
            border: 1px solid transparent; 
            outline: none;
            background-color: transparent;
            color: #cdd6f4;
            font-weight: bold;
            font-size: 14px;
            border-radius: 4px;
        }
        #icon_btn:hover {
            background-color: #313244;
            border: 1px solid #313244;
        }
        #icon_btn:pressed {
            background-color: #45475a;
            border: 1px solid #45475a;
        }

        #files_btn {
            border: none;
            outline: none;
            background-color: transparent;
            color: #cdd6f4;
            font-size: 13px;
            border-radius: 4px;
        }
        #files_btn:hover {
            background-color: #313244;
            border: 1px solid #313244;
        }
        #files_btn:pressed {
            background-color: #45475a;
            border: 1px solid #45475a;
        }

        QSplitter::handle {
            background-color: #313244;
            width: 1px;
        }

        QTreeView {
            background-color: #181825;
            color: #cdd6f4;
            border: none;
            font-size: 13px;
        }
        QTreeView::item {
            padding: 3px 0px;
            border-radius: 4px;
        }
        QTreeView::item:hover {
            background-color: #313244;
        }
        QTreeView::item:selected {
            background-color: #45475a;
            color: #cdd6f4;
        }
        QTreeView::branch {
            background-color: #181825;
        }

        QTabWidget::pane {
            border: none;
            background-color: #1e1e2e;
        }
        QTabBar {
            background-color: #181825;
        }
        QTabBar::tab {
            background-color: #181825;
            color: #6c7086;
            padding: 6px 16px;
            border: none;
            font-size: 13px;
            min-width: 80px;
        }
        QTabBar::tab:selected {
            background-color: #1e1e2e;
            color: #cdd6f4;
            border-top: 2px solid #89b4fa;
        }
        QTabBar::tab:hover:!selected {
            background-color: #313244;
            color: #cdd6f4;
        }

        QTabBar::close-button {
            image: url(:/resources/icons/close.png);
            subcontrol-position: right;
            width: 12px;
            height: 12px;
        }
        QTabBar::close-button:hover {
            background-color: #f38ba8;
            border-radius: 3px;
        }

        QPlainTextEdit {
            background-color: #1e1e2e;
            color: #cdd6f4;
            border: none;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 14px;
            selection-background-color: #45475a;
            selection-color: #cdd6f4;
        }

        QScrollBar:vertical {
            background-color: #1e1e2e;
            width: 8px;
            border: none;
        }
        QScrollBar::handle:vertical {
            background-color: #45475a;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #585b70;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            background-color: #1e1e2e;
            height: 8px;
            border: none;
        }
        QScrollBar::handle:horizontal {
            background-color: #45475a;
            border-radius: 4px;
            min-width: 20px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #585b70;
        }
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            width: 0px;
        }

    )");
}

} // namespace LosStyle