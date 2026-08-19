// Copyright (c) 2026 LosAngelous (shengjie.lin)
#pragma once

#include <QString>

namespace LosStyle
{
    /**
     * @brief LosMDPreview_styleTemplate
     * - QTextBrowser 控件级 QSS(背景/边框/滚动条)
     * - 占位符由 LosThemeManager::buildExtraQss 替换为当前主题色
     * - 注意: QSS 只能控制控件外观, 文档内部(标题/代码块)的颜色见下方 docCss
     */
    inline const QString LosMDPreview_styleTemplate()
    {
        return QString(R"(
        QTextBrowser {
            background-color: @panelBg@;
            color: @foreground@;
            border: none;
            padding: 12px;
            font-family: @fontFamily@;
            font-size: 14px;
        }
        QScrollBar:vertical {
            background-color: transparent;
            width: 10px;
            border: none;
            margin: 2px;
        }
        QScrollBar::handle:vertical {
            background-color: @muted@;
            border-radius: 5px;
            min-height: 24px;
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

    /**
     * @brief LosMDPreview_docCssTemplate
     * - QTextDocument 内部 CSS(由 setDefaultStyleSheet 应用)
     * - 控制 setMarkdown 渲染出的正文/标题/代码/链接颜色
     * - 占位符同样可用 LosThemeManager::buildExtraQss 替换
     */
    inline const QString LosMDPreview_docCssTemplate()
    {
        return QString(R"(
        body 
        { 
            color: @panelBg@; 
            font-family: @fontFamily@; 
            background-color: @background@;
        }

        h1, h2, h3, h4, h5, h6 
        { 
            color: @primary@; 
        }

        a 
        { 
            color: @success@; 
        }

        code 
        { 
            color: @success@; 
            background-color: @selection@; 
        }

        pre
        { 
            background-color: @panelBg@; 
        }

        blockquote 
        { 
            color: @muted@; 
        }
    )");
    }

} /* namespace LosStyle */
