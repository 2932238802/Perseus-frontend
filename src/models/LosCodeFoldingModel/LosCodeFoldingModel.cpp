
#include "LosCodeFoldingModel.h"

#include "core/LosTree/LosTreeSitterFoldRange/LosTreeSitterFoldRange.h"

namespace LosModel
{
    /**
     * @brief 更新语法分析得到的折叠范围

    使用最新语法树生成的折叠范围
    替换 Model 当前保存的范围
    因为 Model 不应该依赖 Tree-sitter
        接收新的折叠范围
        ↓
    替换 L_ranges
        ↓
    保留仍然存在的折叠状态
        ↓
    删除已经不存在的折叠状态

     * @param ranges
     */
    void LosCodeFoldingModel::rebuild(const QVector<LosCore::LosTreeSitterFoldRange> &ranges)
    {
        // 重构 L_ranges
        // 重构 L_collapsedLines;
        QVector<LosCore::LosTreeSitterFoldRange> validRanges;
        QSet<int> validStartLines;
        for (const auto &range : ranges)
        {
            if (!range.isValid())
            {
                continue;
            }

            validRanges.append(range);
            validStartLines.insert(range.L_startLine);
        }
        L_ranges = std::move(validRanges);
        QSet<int> retainedCollapsedLines;
        for (const int collapsedLine : L_collapsedLines)
        {
            if (validStartLines.contains(collapsedLine))
            {
                retainedCollapsedLines.insert(collapsedLine);
            }
        }
        L_collapsedLines = std::move(retainedCollapsedLines);
    }



    /**
     * @brief 读取当前折叠范围

    LosTreeSitterFoldingProvider::collect()
        生成 ranges
            ↓
    LosCodeFoldingModel::rebuild()
        保存到 L_ranges
            ↓
    LosCodeFoldingModel::foldRanges()
        提供给 EditorUi

     * @return const QVector<LosCore::LosTreeSitterFoldRange>&
     */
    const QVector<LosCore::LosTreeSitterFoldRange> &LosCodeFoldingModel::foldRanges() const
    {
        return L_ranges;
    }



    /**
     * @brief 查询某个范围当前是否折叠
     *
     * @param startLine
     * @return true
     * @return false
     */
    bool LosCodeFoldingModel::isCollapsed(int startLine) const
    {
        // L_collapsedLines 开始线 在不在
        return L_collapsedLines.contains(startLine);
    }



    /**
     * @brief 切换某个范围的折叠状态

    确认 startLine 对应的折叠范围存在
    ↓
如果当前已折叠，则展开
如果当前已展开，则折叠
    ↓
返回切换后的状态

     * @param startLine
     * @return true
     * @return false
     */
    bool LosCodeFoldingModel::toggleFold(int startLine)
    {
        bool rangeExists = false;
        for (const LosCore::LosTreeSitterFoldRange &range : L_ranges)
        {
            if (range.L_startLine == startLine)
            {
                rangeExists = true;
                break;
            }
        }
        if (!rangeExists)
        {
            return false;
        }
        if (L_collapsedLines.contains(startLine))
        {
            L_collapsedLines.remove(startLine);
        }
        else
        {
            L_collapsedLines.insert(startLine);
        }
        return L_collapsedLines.contains(startLine);
    }
} // namespace LosModel