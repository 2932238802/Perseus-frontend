// Copyright (c) 2026 LosAngelous (shengjie.lin)



#pragma once


namespace LosCommon
{
    namespace LosEditorUi_Constants
    {
        /* ---------------- 行号区 ---------------- */
        inline constexpr const char *LINENUMBER_CURLINE   = "#bd93f9";
        inline constexpr const char *LINENUMBER_UNCURLINE = "#6272a4";
        inline constexpr const char *LINENUMBER_BG_COLOR  = "#21222c";

        /* ---------------- 缩进 ---------------- */
        inline constexpr const char *BASE_INDENT = "    ";

        /* ---------------- 诊断下划线颜色 (LSP diagnostic severity) ---------------- */
        inline constexpr const char *DIAG_INFO_COLOR  = "#8be9fd";
        inline constexpr const char *DIAG_ERROR_COLOR = "#ff5555";
        inline constexpr const char *DIAG_HINT_COLOR  = "#6272a4";
        inline constexpr const char *DIAG_WARN_COLOR  = "#ffb86c";

        /* ---------------- 当前行 / 括号匹配 / hover 下划线 ---------------- */
        inline constexpr const char *CURLINE_HL_COLOR       = "#44475a";
        inline constexpr const char *BRACKET_MATCH_BG_COLOR = "#50fa7b";
        inline constexpr const char *HOVER_UNDERLINE_COLOR  = "#8be9fd";

        /* ---------------- 搜索高亮 ---------------- */
        inline constexpr const char *SEARCH_HL_BG_COLOR = "#575b70";

        /* ---------------- Hover 浮窗样式 ---------------- */
        inline constexpr const char *HOVER_POP_STYLE = R"(
                QLabel {
                    background-color: #21222c;
                    color: #f8f8f2;
                    border: 1px solid #bd93f9;
                    border-radius: 6px;
                    padding: 6px 10px;
                    font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
                    font-size: 13px;
                }
            )";

        /* ---------------- Hover Markdown -> HTML 渲染模板 ---------------- */
        /*
         * 用于 onHover_Clangd / onHover_Rust 将 LSP 返回的 markdown 转成 rich text.
         * 保持与其它 Str 常量一致的字符串常量形式.
         */
        inline constexpr const char *MD_FENCE_CPP   = "```cpp\n";
        inline constexpr const char *MD_FENCE_C     = "```c\n";
        inline constexpr const char *MD_FENCE_RUST  = "```rust\n";
        inline constexpr const char *MD_FENCE_PLAIN = "```";

        inline constexpr const char *HOVER_CODE_BLOCK_OPEN     = "<pre style='color:#8be9fd; font-family:Consolas; margin: 5px 0;'>";
        inline constexpr const char *HOVER_CODE_BLOCK_CLOSE    = "</pre>";
        inline constexpr const char *HOVER_INLINE_CODE_REPLACE = "<code style='color:#f1fa8c; background-color:#44475a; padding:2px 4px; "
                                                                 "border-radius:3px;'>\\1</code>";
        inline constexpr const char *HOVER_BOLD_REPLACE        = "<b>\\1</b>";
        inline constexpr const char *HOVER_LINE_BREAK          = "<br>";

        /* markdown 语法正则 */
        inline constexpr const char *HOVER_BOLD_REGEX        = "\\*\\*(.*?)\\*\\*";
        inline constexpr const char *HOVER_INLINE_CODE_REGEX = "`([^`]+)`";
    }; // namespace LosEditorUi_Constants
} // namespace LosCommon
