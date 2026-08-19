// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QString>

/*
 * LosAgentKeyUi 样式模板
 * - 与 perseus_style.qss 共用同一套 @token@ 占位符
 * - 运行时由 LosCore::LosThemeManager::buildExtraQss(模板, 主题名) 替换为当前主题色
 * - 参照 LosAuthUi_style.h
 *
 * 可用 token:
 *   @background@  @panelBg@  @borderStrong@  @selection@  @foreground@
 *   @primary@  @primaryHover@  @primaryPressed@  @highlightFg@
 *   @danger@  @success@  @muted@
 *
 * objectName 约定:
 *   #LosAgentKeyUi      根容器
 *   #agentKeyTitle      标题
 *   #agentKeyTip        提示行
 *   #agentKeyPrimaryBtn 保存按钮
 *   #agentKeyGhostBtn   取消/获取按钮
 */
namespace LosStyle
{
    inline const QString losAgentKey_getStyleTemplate()
    {
        return QStringLiteral(R"(
        #LosAgentKeyUi {
            background-color: @background@;
        }
        #LosAgentKeyUi QLabel {
            color: @foreground@;
            background-color: transparent;
        }
        #agentKeyTitle {
            font-size: 20px;
            font-weight: bold;
            color: @foreground@;
        }
        #agentKeyTip {
            font-size: 12px;
            min-height: 18px;
            padding: 4px 2px;
        }
        #LosAgentKeyUi QLineEdit {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 9px 12px;
            min-height: 18px;
            selection-background-color: @selection@;
        }
        #LosAgentKeyUi QLineEdit:focus {
            border: 1px solid @primary@;
        }
        #LosAgentKeyUi QComboBox {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 8px 12px;
            min-height: 18px;
        }
        #LosAgentKeyUi QComboBox:hover {
            border: 1px solid @primary@;
        }
        #LosAgentKeyUi QComboBox::drop-down {
            border: none;
            width: 26px;
        }
        #LosAgentKeyUi QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid @primary@;
            margin-right: 10px;
        }
        #LosAgentKeyUi QComboBox QAbstractItemView {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 4px;
            selection-background-color: @selection@;
            outline: none;
        }
        #agentKeyPrimaryBtn {
            background-color: @primary@;
            color: @highlightFg@;
            border: none;
            border-radius: 6px;
            padding: 10px 24px;
            min-height: 18px;
            font-weight: bold;
        }
        #agentKeyPrimaryBtn:hover {
            background-color: @primaryHover@;
        }
        #agentKeyPrimaryBtn:pressed {
            background-color: @primaryPressed@;
        }
        #agentKeyGhostBtn {
            background-color: transparent;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 10px 24px;
            min-height: 18px;
        }
        #agentKeyGhostBtn:hover {
            border: 1px solid @primary@;
            color: @primary@;
        }
    )");
    }
} /* namespace LosStyle */