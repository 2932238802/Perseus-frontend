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
 *   #agentAddBtn        添加配置按钮
 *   #agentRefreshBtn    刷新配置按钮
 *   #agentProviderCombo 厂商下拉
 *   #agentModelCombo    模型下拉
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
            font-weight: 700;
            letter-spacing: 0.5px;
            padding-left: 2px;
        }
        #agentAddBtn {
            color: @foreground@;
            background-color: transparent;
            border: 1px solid @borderStrong@;
            border-radius: 8px;
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
            border: 1px solid @primaryPressed@;
        }
        #agentRefreshBtn {
            color: @foreground@;
            background-color: transparent;
            border: 1px solid @borderStrong@;
            border-radius: 8px;
            padding: 0;
        }
        #agentRefreshBtn:hover {
            background-color: @primary@;
            color: @highlightFg@;
            border: 1px solid @primary@;
        }
        #agentRefreshBtn:pressed {
            background-color: @primaryPressed@;
            color: @highlightFg@;
            border: 1px solid @primaryPressed@;
        }
        #agentChatView {
            background-color: @background@;
            border: none;
            padding: 4px 2px;
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
            border-radius: 8px;
            padding: 5px 10px;
            min-height: 22px;
        }
        #agentProviderCombo:hover, #agentModelCombo:hover {
            border: 1px solid @primary@;
        }
        #agentProviderCombo:focus, #agentModelCombo:focus {
            border: 1px solid @primary@;
        }
        #agentProviderCombo::drop-down, #agentModelCombo::drop-down {
            border: none;
            width: 20px;
        }
        #agentProviderCombo QAbstractItemView, #agentModelCombo QAbstractItemView {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 8px;
            padding: 4px;
            selection-background-color: @selection@;
            selection-color: @primary@;
            outline: none;
        }
        #agentBubbleUser {
            background-color: @primary@;
            color: @highlightFg@;
            border-radius: 14px;
            line-height: 150%;
            padding: 8px 12px;
        }
        #agentBubbleAgent {
            background-color: @panelBg@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 14px;
            line-height: 160%;
            padding: 4px 10px;
        }
        #agentBubbleAgent QWidget {
            background-color: transparent;
        }
        #agentBubbleAgent a {
            color: @primary@;
            text-decoration: none;
        }
        #agentInput {
            background-color: @background@;
            color: @foreground@;
            border: 1px solid @borderStrong@;
            border-radius: 10px;
            padding: 9px 12px;
            selection-background-color: @selection@;
        }
        #agentInput:focus {
            border: 1px solid @primary@;
        }
        #agentSendBtn {
            background-color: @primary@;
            color: @highlightFg@;
            border: none;
            border-radius: 10px;
            padding: 9px 20px;
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
            width: 8px;
            border: none;
            margin: 2px 1px;
        }
        #LosAgentUi QScrollBar::handle:vertical {
            background-color: @borderStrong@;
            border-radius: 4px;
            min-height: 36px;
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
