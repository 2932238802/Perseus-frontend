// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QString>

/*
 * LosAuthUi 样式模板
 * - 与 perseus_style.qss 共用同一套 @token@ 占位符
 * - 运行时由 LosCore::LosThemeManager::buildExtraQss(模板, 主题名) 替换为当前主题色
 * - 抽离到独立头文件, 保持 逻辑(.cpp) 与 样式 分离 (参照 LosMDPreview_style.h)
 *
 * 可用 token:
 *   @background@  @panelBg@  @borderStrong@  @selection@  @foreground@
 *   @primary@  @primaryHover@  @primaryPressed@  @highlightFg@
 *   @danger@  @success@
 */
namespace LosStyle
{
    inline const QString losAuth_getStyleTemplate()
    {
        return QStringLiteral(R"(
        #LosAuthUi {
            background-color: @background@;
        }
        #LosAuthUi QLabel {
            color: @foreground@;
            background-color: transparent;
        }
        #authTitle {
            font-size: 20px;
            font-weight: bold;
            color: @foreground@;
        }
        #authTip {
            font-size: 12px;
        }
        #LosAuthUi QLineEdit {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 8px 10px;
            selection-background-color: @selection@;
        }
        #LosAuthUi QLineEdit:focus {
            border: 1px solid @primary@;
        }
        #authPrimaryBtn {
            background-color: @primary@;
            color: @highlightFg@;
            border: none;
            border-radius: 6px;
            padding: 9px 0;
            font-weight: bold;
        }
        #authPrimaryBtn:hover {
            background-color: @primaryHover@;
        }
        #authPrimaryBtn:pressed {
            background-color: @primaryPressed@;
        }
        #authLinkBtn {
            background-color: transparent;
            color: @primary@;
            border: none;
            padding: 4px;
        }
        #authLinkBtn:hover {
            color: @primaryHover@;
            text-decoration: underline;
        }
    )");
    }
} /* namespace LosStyle */
