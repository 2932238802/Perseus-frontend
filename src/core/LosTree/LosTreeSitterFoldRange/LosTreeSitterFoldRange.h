#pragma once

#include <QString>

namespace LosCore
{
    /**
     * @brief LosTreeSitterNodeInfo -> LosTreeSitterFoldRange
     */
    struct LosTreeSitterFoldRange
    {
        int L_startLine = -1;
        int L_endLine = -1;
        QString L_kind;

        bool isValid() const
        {
            return L_startLine >= 0 &&
                   L_endLine > L_startLine;
        }
    };
}
