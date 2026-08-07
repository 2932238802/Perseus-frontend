#pragma once
#include <QString>

namespace LosCore
{
    /**
     * @brief NodeInfo
     * tree site -> node info
     */
    struct LosTreeSitterNodeInfo
    {
        QString L_type; // LosTreeSitterNodeInfo_Constants
        int L_startLine   = -1;
        int L_endLine     = -1;
        int L_startColumn = -1;
        int L_endColumn   = -1;

        // 匿名 标点是 括弧 或者 逗号句号
        bool L_isNamed  = false;
        bool L_hasError = false;
    };
} // namespace LosCore