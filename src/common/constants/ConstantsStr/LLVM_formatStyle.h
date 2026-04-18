#pragma once

namespace LosCommon
{

    /*
     * clang-format 文本格式化配置
     */
    namespace LLVM_formatStyle
    {
        inline constexpr const char *FORMAT_STYLE = R"({
            Language: Cpp,
            BasedOnStyle: LLVM,
            ColumnLimit: 120,
            MaxEmptyLinesToKeep: 3,
            AlignConsecutiveAssignments: true,
            AlignTrailingComments: true,
            AlignConsecutiveMacros: true,
            PointerAlignment: Right,
            IndentWidth: 4,
            TabWidth: 4,
            UseTab: Never,
            AllowShortBlocksOnASingleLine: true,
            AllowShortFunctionsOnASingleLine: Inline,
            SortIncludes: true,
            BreakBeforeBraces: Allman,
            NamespaceIndentation: All,     
            FixNamespaceComments: true
        })";
    } /* namespace LLVM_formatStyle */
}