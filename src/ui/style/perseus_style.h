#pragma once
#include <QString>

namespace LosStyle {

inline const QString perseus_getStyle() {
    return QString(R"(

        QMainWindow {
            background-color: #1e1e2e; 
        }
        
        * {
            outline: none;
        }

        QToolTip {
            background-color: #313244;  
            color: #cdd6f4;             
            border: 1px solid #45475a;  
            border-radius: 4px;
            padding: 4px 8px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 12px;
        }

        #toolbar_widget {
            background-color: #181825; 
            border-bottom: 1px solid #313244;
        }

        #icon_btn, #files_btn, #run_singleFile_btn {
            border: 1px solid transparent; 
            background-color: rgba(255, 255, 255, 0.01);
            color: #cdd6f4; 
            border-radius: 4px;
            padding: 4px 12px; 
            font-size: 13px;   
            font-family: "Microsoft YaHei", sans-serif;
        }
        
        #icon_btn {
            font-weight: bold;
            color: #89b4fa; 
            padding: 4px 0px;   
            min-width: 32px;     
            text-align: center;  
            font-size: 15px; 
        }


        #icon_btn:hover, #files_btn:hover, #run_singleFile_btn:hover {
            background-color: #313244;
            border: 1px solid #313244;
            color: #ffffff; /* 鼠标放上去时，字体变亮，增加交互感 */
        }
        #icon_btn:pressed, #files_btn:pressed, #run_singleFile_btn:pressed {
            background-color: #45475a;
            border: 1px solid #45475a;
        }

        QSplitter::handle {
            background-color: #1e1e2e;
            margin: 1px;
        }
        QSplitter::handle:horizontal:hover, QSplitter::handle:vertical:hover {
            background-color: #89b4fa;
        }

        QTreeView {
            background-color: #181825;
            color: #bac2de; 
            border: none;
            font-size: 13px;
            show-decoration-selected: 1; 
        }
        QTreeView::item {
            padding: 4px 0px;
            border-radius: 4px;
            margin: 0px 4px; 
        }
        QTreeView::item:hover {
            background-color: #313244;
            color: #cdd6f4;
        }
        QTreeView::item:selected {
            background-color: #45475a;
            color: #89b4fa; 
            font-weight: bold;
        }
        QTreeView::branch {
            background-color: transparent;
        }

        #editor_tabwidget::pane {
            border: none;
            background-color: #1e1e2e;
        }
        #editor_tabwidget QTabBar {
            background-color: #181825;
        }
        #editor_tabwidget QTabBar::tab {
            background-color: #181825;
            color: #6c7086;
            padding: 8px 16px;
            border: none;
            border-right: 1px solid #1e1e2e; 
            font-size: 13px;
            min-width: 90px;
        }
        #editor_tabwidget QTabBar::tab:selected {
            background-color: #1e1e2e;
            color: #89b4fa; 
            border-top: 2px solid #89b4fa; 
        }
        #editor_tabwidget QTabBar::tab:hover:!selected {
            background-color: #313244;
            color: #cdd6f4;
        }
        #editor_tabwidget QTabBar::close-button {
            subcontrol-position: right;
            margin-right: 2px;
        }
        #editor_tabwidget QTabBar::close-button:hover {
            background-color: #f38ba8; 
            border-radius: 2px;
        }

        #bottom_tabwidget::pane {
            border-top: 1px solid #313244; 
            background-color: #181825;
        }
        #bottom_tabwidget QTabBar {
            background-color: #1e1e2e;
        }
        #bottom_tabwidget QTabBar::tab {
            background-color: transparent;
            color: #6c7086;
            padding: 4px 16px 2px 16px; 
            border: none;
            border-bottom: 2px solid transparent;
            font-size: 12px;
            text-transform: uppercase; 
        }
        #bottom_tabwidget QTabBar::tab:selected {
            color: #cdd6f4;
            border-bottom: 2px solid #89b4fa; 
        }
        #bottom_tabwidget QTabBar::tab:hover:!selected {
            color: #a6adc8;
        }

        QPlainTextEdit {
            background-color: #1e1e2e;
            color: #cdd6f4;
            border: none;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 14px;
            selection-background-color: #45475a;
            selection-color: #cdd6f4;
            padding: 4px;
        }
        
        #output_plaintextedit {
            background-color: #181825;
            font-size: 13px; 
        }

        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background-color: rgba(69, 71, 90, 0.6); 
            border-radius: 6px;
            min-height: 30px;
            margin: 2px; 
        }
        QScrollBar::handle:vertical:hover {
            background-color: rgba(88, 91, 112, 0.9);
        }
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border: none;
            margin: 0px;
        }
        QScrollBar::handle:horizontal {
            background-color: rgba(69, 71, 90, 0.6);
            border-radius: 6px;
            min-width: 30px;
            margin: 2px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: rgba(88, 91, 112, 0.9);
        }
        QScrollBar::add-line, QScrollBar::sub-line {
            height: 0px; width: 0px; 
        }
        QScrollBar::add-page, QScrollBar::sub-page {
            background-color: transparent; 
        }

    )");
}

} // namespace LosStyle
