#pragma once
#include <QString>

namespace LosStyle
{

inline const QString perseus_getStyle()
{
    return QString(R"(
        /* 
           Geek Black & White Theme 
           Atmospheric, sleek, high-contrast, professional
        */
        
        QMainWindow {
            background-color: #121212; 
        }
        
        * {
            outline: none;  
        }

        /* ----- Global QTabWidget fixes for documentMode white lines ----- */
        QTabWidget {
            border: none;
        }

        QTabBar::base {
            border: none;
            border-bottom: 0px;
            border-top: 0px;
        }

        /* ----- Tooltips ----- */
        QToolTip {
            background-color: #1e1e1e;  
            color: #ffffff;             
            border: 1px solid #333333;  
            border-radius: 4px;
            padding: 4px 8px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 0.9em;
        }

        /* ----- Toolbars ----- */
        #toolbar_widget {
            background-color: #1e1e1e; 
            border-bottom: 1px solid #333333;
        }

        #icon_btn, #files_btn, #run_singleFile_btn, #setting_btn {
            border: 1px solid transparent; 
            background-color: transparent;
            color: #d4d4d4; 
            border-radius: 4px;
            padding: 4px 12px; 
            font-size: 16px;
            font-family: "Microsoft YaHei", sans-serif;
        }
        
        #icon_btn {
            font-weight: bold;
            color: #ffffff; 
            padding: 4px 0px;   
            min-width: 32px;     
            text-align: center;  
            font-size: 1.15em; 
        }

        #icon_btn:hover, #files_btn:hover, #run_singleFile_btn:hover, #setting_btn:hover {
            background-color: #2a2d2e;
            border: 1px solid #333333;
            color: #ffffff; 
        }

        #icon_btn:pressed, #files_btn:pressed, #run_singleFile_btn:pressed, #setting_btn:pressed {
            background-color: #37373d;
            border: 1px solid #4a4a4a;
        }

        #activity_bar {
            background-color: #181818;
            border-right: 1px solid #333333;
        }


        #activity_bar QPushButton:hover {
            background: transparent;
            background-color: transparent;
        }

        #activity_bar QPushButton:checked {
            border-left: 3px solid #ffffff;
            background: transparent;
            background-color: transparent;
        }

        #activity_bar QPushButton:pressed {
            background: transparent;
            background-color: transparent;
        }



        #act_explorer_btn {

            image: url(:/icons/files_black.png);
        }
        #act_explorer_btn:hover, #act_explorer_btn:checked {
            image: url(:/icons/files_white.png);
        }

        #act_extensions_btn {
            image: url(:/icons/chajian_black.png);
        }
        #act_extensions_btn:hover, #act_extensions_btn:checked {
            image: url(:/icons/chajian_white.png);
        }

        #project_btn {
            background-color: transparent;
            color: #a0a0a0; 
            border: none;
            padding: 6px 16px;
            font-size: 14px;
            font-family: "Microsoft YaHei", sans-serif;
            spacing: 8px; 
        }

        #project_btn:hover {
            color: #ffffff;  
        }

        #project_btn:checked {
            color: #ffffff;  
            font-weight: bold;
        }

        #project_btn::indicator {
            width: 14px;
            height: 14px;
            border-radius: 7px;
            border: 2px solid #555555; 
            background-color: transparent;
        }

        #project_btn::indicator:hover {
            border-color: #888888; 
        }

        #project_btn::indicator:checked {
            border-color: #ffffff; 
            background-color: #ffffff; 
        }

        #project_btn::indicator:checked:hover {
            background-color: #d4d4d4;
            border-color: #d4d4d4; 
        }

        /* ----- Splitters ----- */
        QSplitter::handle {
            background-color: #121212;
            margin: 1px;
        }
            
        QSplitter::handle:horizontal:hover, QSplitter::handle:vertical:hover {
            background-color: #555555;
        }

        /* ----- TreeView ----- */
        QTreeView {
            background-color: #1e1e1e;
            color: #cccccc; 
            border: none;
            show-decoration-selected: 1; 
        }
        
        QTreeView::item {
            padding: 4px 0px;
            border-radius: 4px;
            margin: 0px 4px; 
        }
        
        QTreeView::item:hover {
            background-color: #2a2d2e;
            color: #ffffff;
        }
        
        QTreeView::item:selected {
            background-color: #37373d;
            color: #ffffff; 
            font-weight: bold;
        }
        
        QTreeView::branch {
            background-color: transparent;
        }

        /* ----- Editor TabWidget ----- */
        #editor_tabwidget::pane {
            border: 0px solid transparent;
            background-color: #121212;
            top: -1px; /* Remove white line */
        }
        
        #editor_tabwidget QTabBar {
            background-color: #1e1e1e;
            border: none;
        }
        
        #editor_tabwidget QTabBar::tab {
            background-color: #1e1e1e;
            color: #888888;
            padding: 8px 16px;
            border: none;
            border-right: 1px solid #333333; 
            min-width: 90px;
        }
        
        #editor_tabwidget QTabBar::tab:selected {
            background-color: #121212;
            color: #ffffff; 
            border-top: 2px solid #ffffff; 
        }
        
        #editor_tabwidget QTabBar::tab:hover:!selected {
            background-color: #2a2d2e;
            color: #d4d4d4;
        }
        
        #editor_tabwidget QTabBar::close-button {
            subcontrol-position: right;
            margin-right: 2px;
        }
        
        #editor_tabwidget QTabBar::close-button:hover {
            background-color: #cc0000; 
            border-radius: 2px;
        }

        #bottom_tabwidget::pane {
            border: 0px solid transparent; 
            background-color: #1e1e1e;
            top: -1px;
            bottom: -1px;
        }
        
        #bottom_tabwidget > QTabBar {
            background-color: #121212;
            border: none;
        }
        
        #bottom_tabwidget > QTabBar::tab {
            background-color: transparent;
            color: #888888;
            padding: 6px 16px; 
            border: none;
            font-size: 0.9em;
            text-transform: uppercase; 
        }
        
        #bottom_tabwidget > QTabBar::tab:selected {
            color: #ffffff;
            background-color: #1e1e1e;
        }
        
        #bottom_tabwidget > QTabBar::tab:hover:!selected {
            color: #d4d4d4;
            background-color: #2a2d2e;
        }

        /* ----- Terminal & Output ----- */
        QPlainTextEdit {
            background-color: #121212;
            color: #d4d4d4;
            border: none;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 1.1em;
            selection-background-color: #444444;
            selection-color: #ffffff;
            padding: 4px;
        }
        
        #output_plaintextedit {
            background-color: #1e1e1e;
        }

        #terminal_tabs::pane {
            border: 0px solid transparent;
            background-color: #121212;
            top: -1px;
        }
        
        #terminal_tabs QTabBar {
            background-color: #1e1e1e;
            border: none;
        }
        
        #terminal_tabs QTabBar::tab {
            background-color: #1e1e1e;
            color: #888888;
            padding: 6px 12px;
            border: none;
            border-right: 1px solid #333333;
            min-width: 60px;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 12px;
        }
        
        #terminal_tabs QTabBar::tab:selected {
            background-color: #121212;
            color: #ffffff;
        }
        
        #terminal_tabs QTabBar::tab:hover:!selected {
            background-color: #2a2d2e;
            color: #d4d4d4;
        }
        
        #terminal_tabs QTabBar::close-button {
            subcontrol-position: right;
            margin-right: 4px;
        }
        
        #terminal_tabs QTabBar::close-button:hover {
            background-color: #cc0000;
            border-radius: 2px;
        }

        /* ----- Extensions (LosPlugUi) ----- */
        #LosPlugUi {
            background-color: #1e1e1e;
        }

        #LosPlugUi QLabel#titleLabel {
            color: #cccccc;
            font-size: 11px;
            font-weight: bold;
            font-family: "JetBrains Mono", "Consolas", monospace;
            padding-left: 4px;
        }

        #LosPlugUi QLineEdit#searchLineEdit {
            background-color: #2a2d2e;
            color: #ffffff;
            border: 1px solid #333333;
            border-radius: 4px;
            padding: 6px 10px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 13px;
        }

        #LosPlugUi QLineEdit#searchLineEdit:focus {
            border: 1px solid #007acc;
            background-color: #1e1e1e;
        }

        #LosPlugUi QListWidget#extensionsList {
            background-color: transparent;
            color: #d4d4d4;
            font-family: "Microsoft YaHei", sans-serif;
            outline: none;
            border: none;
        }

        #LosPlugUi QListWidget::item {
            padding: 8px;
            border-radius: 4px;
            border-bottom: 1px solid transparent;
            margin: 2px 4px;
        }

        #LosPlugUi QListWidget::item:hover {
            background-color: #2a2d2e;
        }

        #LosPlugUi QListWidget::item:selected {
            background-color: #37373d;
            color: #ffffff;
            border-left: 3px solid #007acc;
            border-radius: 0px;
        }

        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border: none;
            margin: 0px;
        }
        
        QScrollBar::handle:vertical {
            background-color: rgba(85, 85, 85, 0.6); 
            border-radius: 6px;
            min-height: 30px;
            margin: 2px; 
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: rgba(136, 136, 136, 0.9);
        }
        
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border: none;
            margin: 0px;
        }
        
        QScrollBar::handle:horizontal {
            background-color: rgba(85, 85, 85, 0.6);
            border-radius: 6px;
            min-width: 30px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: rgba(136, 136, 136, 0.9);
        }
        
        QScrollBar::add-line, QScrollBar::sub-line {
            height: 0px; width: 0px; 
        }
        
        QScrollBar::add-page, QScrollBar::sub-page {
            background-color: transparent; 
        }

        /* ----- Settings Dialog ----- */
        QDialog#LosSettingsUi {
            background-color: #121212;
        }

        #category_list {
            background-color: #1e1e1e;
            border: none;
            border-right: 1px solid #333333;
            padding-top: 10px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 14px;
            outline: none; 
        }

        #category_list::item {
            color: #cccccc;
            padding: 10px 15px;
            border-left: 3px solid transparent; 
        }

        #category_list::item:hover {
            background-color: #2a2d2e;
            color: #ffffff;
        }

        #category_list::item:selected {
            background-color: #121212;
            color: #ffffff;
            font-weight: bold;
            border-left: 3px solid #ffffff;
        }

        #pages_stack {
            background-color: #121212;
            border: none;
        }

        #label_title_build {
            color: #ffffff;
            border-bottom: 1px solid #333333;
            padding-bottom: 5px;
            font-family: "Microsoft YaHei", sans-serif;
        }

        #label_cmake {
            color: #a0a0a0;
            font-size: 14px;
            font-family: "Microsoft YaHei", sans-serif;
        }

        #edit_cmake_path {
            background-color: #000000;
            color: #ffffff;
            border: 1px solid #333333;
            border-radius: 4px;
            padding: 6px 10px;
            font-family: "JetBrains Mono", "Consolas", monospace;
            font-size: 13px;
        }
        
        #edit_cmake_path:hover {
            border: 1px solid #555555;
        }
        
        #edit_cmake_path:focus {
            border: 1px solid #ffffff;
            background-color: #1e1e1e;
        }

        #buttonBox QPushButton {
            background-color: #2a2d2e;
            color: #d4d4d4;
            border: 1px solid transparent;
            border-radius: 4px;
            padding: 6px 16px;
            min-width: 60px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 13px;
        }
        
        #buttonBox QPushButton:hover {
            background-color: #37373d;
            color: #ffffff;
        }
        
        #buttonBox QPushButton:pressed {
            background-color: #4a4a4a;
            border: 1px solid #ffffff;
        }

        #buttonBox QPushButton[text="OK"] {
            background-color: #ffffff;
            color: #000000;
            font-weight: bold;
        }
        
        #buttonBox QPushButton[text="OK"]:hover {
            background-color: #d4d4d4;
        }


            #LosPluginDetailUi {
            background-color: #121212;
        }

        #LosPluginDetailUi {
            background-color: #121212;
        }

        #LosPluginDetailUi #label_icon {
            background-color: #1e1e1e;
            border-radius: 8px;
        }

        #LosPluginDetailUi #label_name {
            color: #ffffff;
            font-size: 24px;
            font-weight: bold;
            font-family: "Microsoft YaHei", sans-serif;
        }

        #LosPluginDetailUi #label_version {
            color: #888888;
            font-size: 13px;
            font-family: "JetBrains Mono", "Consolas", monospace;
        }

        #LosPluginDetailUi #btn_install {
            background-color: #ffffff;
            color: #000000;
            font-weight: bold;
            border-radius: 3px;
            padding: 6px 28px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 13px;
            min-width: 100px;
            border: none;
        }

        #LosPluginDetailUi #btn_install:hover {
            background-color: #e0e0e0;
        }

        #LosPluginDetailUi #btn_install:pressed {
            background-color: #cccccc;
        }

        #LosPluginDetailUi #btn_install:disabled {
            background-color: #2a2d2e;
            color: #666666;
            font-weight: normal;
        }

        #LosPluginDetailUi #text_description {
            background-color: transparent;
            color: #d4d4d4;
            border: none;
            border-top: 1px solid #2a2d2e;
            margin-top: 20px;
            padding-top: 20px;
            font-family: "Microsoft YaHei", sans-serif;
            font-size: 14px;
            line-height: 1.6;
        }


        #project_dir_label {
            color: #ffffff;            
            font-weight: bold;         
            font-family: "Consolas", "JetBrains Mono", monospace; 
            font-size: 12px;     
            background-color: #252526; 
            border: 1px solid #333333;  
            border-radius: 4px;     
            padding: 4px 12px;         
            margin-right: 10px;        
        }
        
        #project_dir_label:hover {
            background-color: #2a2d2e;
            border: 1px solid #454545;
        }


    )");
}

} // namespace LosStyle
