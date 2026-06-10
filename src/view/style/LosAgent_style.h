// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QString>

/*
 * LosAgentUi 样式模板
 * - 与 perseus_style.qss 共用同一套 @token@ 占位符
 * - 运行时由 LosCore::LosThemeManager::buildExtraQss(模板, 主题名) 替换为当前主题色
 * - 抽离到独立头文件, 保持 逻辑(.cpp) 与 样式 分离 (参照 LosAuthUi_style.h)
 *
 * 可用 token:
 *   @background@  @panelBg@  @borderStrong@  @selection@  @foreground@
 *   @primary@  @primaryHover@  @primaryPressed@  @highlightFg@
 *   @danger@  @success@
 *
 * objectName 约定:
 *   #LosAgentUi         根容器
 *   #agentHeader        标题栏
 *   #agentChatView      对话列表 (QListWidget)
 *   #agentBubbleUser    用户气泡 (靠右)
 *   #agentBubbleAgent   Agent 气泡 (靠左)
 *   #agentInput         输入框
 *   #agentSendBtn       发送按钮
 */
namespace LosStyle
{
    inline const QString losAgent_getStyleTemplate()
    {
        return QStringLiteral(R"(
        #LosAgentUi {
            background-color: @background@;
            border-left: 1px solid @borderStrong@;
        }
        #agentHeaderBar {
            background-color: @panelBg@;
            border-bottom: 1px solid @borderStrong@;
        }
        #agentHeader {
            color: @foreground@;
            background-color: transparent;
            font-size: 13px;
            font-weight: bold;
        }
        #agentAddBtn {
            color: @foreground@;
            background-color: transparent;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            font-size: 18px;
            font-weight: bold;
            padding: 0;
        }
        #agentAddBtn:hover {
            background-color: @primary@;
            color: @highlightFg@;
            border: 1px solid @primary@;
        }
        #agentAddBtn:pressed {
            background-color: @primaryPressed@;
        }
        #agentChatView {
            background-color: @background@;
            border: none;
        }
        #agentChatView::item {
            border: none;
            background: transparent;
        }
        #agentComposer {
            background-color: @panelBg@;
            border-top: 1px solid @borderStrong@;
        }
        #agentProviderCombo, #agentModelCombo {
            background-color: @background@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 4px 8px;
            font-size: 12px;
            min-height: 20px;
        }
        #agentProviderCombo:hover, #agentModelCombo:hover {
            border: 1px solid @primary@;
        }
        #agentProviderCombo::drop-down, #agentModelCombo::drop-down {
            border: none;
            width: 18px;
        }
        #agentProviderCombo QAbstractItemView, #agentModelCombo QAbstractItemView {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            selection-background-color: @selection@;
            selection-color: @primary@;
            outline: none;
        }
        #agentBubbleUser {
            background-color: @primary@;
            color: @highlightFg@;
            border-radius: 10px;
            font-size: 13px;
        }
        #agentBubbleAgent {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 10px;
            font-size: 13px;
        }
        #agentInput {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 6px;
            padding: 7px 10px;
            selection-background-color: @selection@;
        }
        #agentInput:focus {
            border: 1px solid @primary@;
        }
        #agentSendBtn {
            background-color: @primary@;
            color: @highlightFg@;
            border: none;
            border-radius: 6px;
            padding: 7px 16px;
            font-weight: bold;
        }
        #agentSendBtn:hover {
            background-color: @primaryHover@;
        }
        #agentSendBtn:pressed {
            background-color: @primaryPressed@;
        }
        #LosAgentUi QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border: none;
            margin: 0px;
        }
        #LosAgentUi QScrollBar::handle:vertical {
            background-color: @borderStrong@;
            border-radius: 6px;
            min-height: 30px;
            margin: 2px;
        }
        #LosAgentUi QScrollBar::handle:vertical:hover {
            background-color: @primary@;
        }
        #LosAgentUi QScrollBar::add-line, #LosAgentUi QScrollBar::sub-line,
        #LosAgentUi QScrollBar::add-page, #LosAgentUi QScrollBar::sub-page {
            height: 0px;
            width: 0px;
            background-color: transparent;
        }
    )");
    }
} /* namespace LosStyle */
