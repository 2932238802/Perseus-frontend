// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
namespace LosCommon
{
    namespace LosEditorUi_Constants
    {
        /* ---------------- 编辑器 tab 宽度 ---------------- */
        /* setTabStopDistance = TAB_WIDTH_SPACES * fontMetrics.horizontalAdvance(" ") */
        inline constexpr int TAB_WIDTH_SPACES = 4;

        /* ---------------- 行号区 ---------------- */
        /* 行号数字与右边界的间距 (paintEvent 里 width - LINENUMBER_RIGHT_PADDING) */
        inline constexpr int LINENUMBER_RIGHT_PADDING = 5;
        inline constexpr int FOLD_MARKER_WIDTH        = 16;

        /* ---------------- LSP 补全弹窗 ---------------- */
        inline constexpr int COMPLETION_POPUP_PADDING   = 25;
        inline constexpr int COMPLETION_POPUP_MAX_WIDTH = 500;

        /* ---------------- 编辑防抖 ---------------- */
        /* onContentsChange 触发一次 LSP 请求前的防抖间隔 (ms) */
        inline constexpr int DEBOUNCE_INTERVAL_MS = 200;

        /* ---------------- Hover 浮窗尺寸 / 定位 ---------------- */
        inline constexpr int HOVER_POPUP_MARGIN    = 8;
        inline constexpr int HOVER_POPUP_MAX_WIDTH = 600;

        /* 当没有单词矩形时, hover 浮窗相对光标向下的默认偏移 (px) */
        inline constexpr int HOVER_ANCHOR_FALLBACK_Y = 20;

        /* hover 浮窗贴近屏幕四边时留出的安全边距 */
        inline constexpr int HOVER_SCREEN_MARGIN = 4;

        /* 拿不到 QScreen 时的可用区域回退尺寸 */
        inline constexpr int FALLBACK_SCREEN_W = 1920;
        inline constexpr int FALLBACK_SCREEN_H = 1080;
    } // namespace LosEditorUi_Constants
} // namespace LosCommon
