#pragma once
#include <QString>

namespace LosCommon
{
    namespace LosLsp_Constants
    {
        enum DiagnosticSeverity
        {
            Error = 1,
            Warning,
            Information,
            Hint
        };



        enum LosLspType
        {
            /*
             * 初始化
             */
            REQ_INITIALIZE,
            /*
             * 语法补全
             */
            REQ_COMPLETION,
            /*
             * 语法报错
             */
            REQ_CHECK,
            REQ_HOVER,
            REQ_DEFINE,
            REQ_SEMANTIC_HIGHLIGHT
        };



        struct PendingRequest
        {
            QString L_filePath;
            QString L_content;
            QString L_langId;
        };

        /*
         * didChangeWatchedFiles 字段
         * changes 字段的
         * type 整形
         */
        enum LspJson_didChangeWatchedFiles_changes_type
        {
            Created = 1,
            Changed = 2,
            DELETE  = 3,
        };



        struct LosDiagnostic
        {
            int startLine;
            int startChar;
            int endLine;
            int endChar;
            DiagnosticSeverity ds = DiagnosticSeverity::Warning;
            QString message;
        };
    } /* namespace LosLsp_Constants */

} // namespace LosCommon
