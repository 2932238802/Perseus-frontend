#pragma once
#include <QString>

namespace LosStyle
{

    inline const QString LosCommandUi_style()
    {
        return QString(R"(
            #PaletteContainer {
                background-color: #252526; 
                border: 1px solid #454545; 
                border-radius: 8px;        
            }
            
            QLineEdit {
                background-color: transparent;
                color: #cccccc;
                border: none;
                border-bottom: 1px solid #333333;
                padding: 12px 15px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 14px;
            }
            
            QLineEdit:focus {
                border-bottom: 1px solid #007acc; 
            }
            
            QListWidget {
                background-color: transparent;
                color: #cccccc;
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
                background-color: #2a2d2e;
            }
            
            QListWidget::item:selected {
                background-color: #04395e; 
                color: #ffffff;
                border-left: 3px solid #007acc; 
                border-radius: 0px;
            }
        )");
    }

} /* namespace LosStyle */
