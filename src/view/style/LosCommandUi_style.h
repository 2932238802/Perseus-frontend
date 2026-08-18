// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QString>

namespace LosStyle
{
    /**
     * @brief 模板字符串, 占位符 @key@ 由 LosCore::LosThemeManager 替换
     * 替换后再 setStyleSheet 到目标控件
     * @return const QString 
     */
    inline const QString LosCommandUi_styleTemplate()
    {
        return QString(R"(
            #PaletteContainer {
                background-color: @panelBg@;
                border: 1px solid @primary@;
                border-radius: 8px;
            }

            QLineEdit {
                background-color: transparent;
                color: @foreground@;
                border: none;
                border-bottom: 1px solid @selection@;
                padding: 12px 15px;
                selection-background-color: @primary@;
                selection-color: @background@;
            }

            QLineEdit:focus {
                border-bottom: 1px solid @primary@;
            }

            QListWidget {
                background-color: transparent;
                color: @foreground@;
                border: none;
                outline: none;
                padding: 6px;
            }

            QListWidget::item {
                padding: 8px 10px;
                border-radius: 4px;
                margin: 2px 4px;
            }

            QListWidget::item:hover {
                background-color: @selection@;
            }

            QListWidget::item:selected {
                background-color: @selection@;
                color: @primary@;
                border-left: 3px solid @primary@;
                border-radius: 0px;
            }
        )");
    }

} /* namespace LosStyle */
