
#pragma once

namespace LosCommon
{
    namespace LosTreeSitterNodeInfo_Constants
    {
        // 代码块 if for while switch
        inline constexpr const char *NODEINFO_COMPOUND_STATEMENT = "compound_statement";

        // 类 结构体 联合体
        inline constexpr const char *NODEINFO_CLASS_SPECIFIER  = "class_specifier";
        inline constexpr const char *NODEINFO_STRUCT_SPECIFIER = "struct_specifier";
        inline constexpr const char *NODEINFO_UNION_SPECIFIER  = "union_specifier";

        // 命名空间
        inline constexpr const char *NODEINFO_NAMESPACE_DEFINITION = "namespace_definition";

        // 枚举
        inline constexpr const char *NODEINFO_ENUM_SPECIFIER = "enum_specifier";

        // 注释
        inline constexpr const char *NODEINFO_COMMENT = "comment";

        // 宏
        inline constexpr const char *NODEINFO_PREPROC_IF     = "preproc_if";
        inline constexpr const char *NODEINFO_PREPROC_IFDEF  = "preproc_ifdef";
        inline constexpr const char *NODEINFO_PREPROC_IFNDEF = "preproc_ifndef";
        inline constexpr const char *NODEINFO_PREPROC_ELSE   = "preproc_else";
        inline constexpr const char *NODEINFO_PREPROC_ELIF   = "preproc_elif";

        // lambda
        inline constexpr const char *NODEINFO_LAMBDA_EXPRESSION = "lambda_expression";

    } // namespace LosTreeSitterNodeInfo_Constants
} // namespace LosCommon
