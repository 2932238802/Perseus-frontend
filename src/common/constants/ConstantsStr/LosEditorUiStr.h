// Copyright (c) 2026 LosAngelous (shengjie.lin)



#pragma once


namespace LosCommon
{
    namespace LosEditorUi_Constants
    {
        inline constexpr const char *LINENUMBER_CURLINE   = "#bd93f9";
        inline constexpr const char *LINENUMBER_UNCURLINE = "#6272a4";
        inline constexpr const char *BASE_INDENT          = "    ";
        inline constexpr const char *HOVER_POP_STYLE      = R"(
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
    }; // namespace LosEditorUi_Constants
} // namespace LosCommon