#pragma once

#include <QSet>

namespace LosCore
{
    class LosTreeSitterFoldRange;
}
namespace LosModel
{
    class LosCodeFoldingModel
    {
      public:
        void rebuild(const QVector<LosCore::LosTreeSitterFoldRange> &ranges);
        const QVector<LosCore::LosTreeSitterFoldRange> &foldRanges() const;
        bool isCollapsed(int startLine) const;
        bool toggleFold(int startLine);

      private:
        QVector<LosCore::LosTreeSitterFoldRange> L_ranges; // 范围
        QSet<int> L_collapsedLines;                        // 被折叠的起始行号
    };

} // namespace LosModel