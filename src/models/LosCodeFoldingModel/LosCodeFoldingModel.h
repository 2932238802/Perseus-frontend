#pragma once

#include <QSet>

namespace LosCore
{
    class LosTreeSitterFoldRange;
}
namespace LosCommon
{
    namespace LosCodeFoldingModel_Constants
    {
        enum class FoldMarkerState;
    }
} // namespace LosCommon


namespace LosModel
{
    class LosCodeFoldingModel
    {
      public:
        void rebuild(const QVector<LosCore::LosTreeSitterFoldRange> &ranges);
        const QVector<LosCore::LosTreeSitterFoldRange> &foldRanges() const;
        bool isCollapsed(int startLine) const;       //
        bool toggleFold(int startLine);              // 折叠
        bool hasFoldRangeStartingAt(int line) const; // 判断一下 当前行 是不是可折叠

      public: // get 函数
        LosCommon::LosCodeFoldingModel_Constants::FoldMarkerState getLineFolderState(int line) const;

      private:
        QVector<LosCore::LosTreeSitterFoldRange> L_ranges; // 范围
        QSet<int> L_collapsedLines;                        // 被折叠的起始行号
    };

} // namespace LosModel