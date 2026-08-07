#pragma once

#include "core/LosTree/LosTreeSitterFoldRange/LosTreeSitterFoldRange.h"

#include <QVector>

namespace LosCore
{
    class LosTreeSitterDocument;

    /**
     * @brief 筛选哪些类型可折叠
     */
    class LosTreeSitterFoldingProvider
    {
        /**
         * @brief collect
         */
      public:
        QVector<LosTreeSitterFoldRange> collect(const LosTreeSitterDocument &document) const;
    };
} // namespace LosCore
