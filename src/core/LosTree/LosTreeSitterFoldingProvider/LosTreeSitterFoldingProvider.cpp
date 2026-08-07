#include "LosTreeSitterFoldingProvider.h"

#include "common/constants/ConstantsStr/LosTreeSitterNodeInfoStr.h"
#include "core/LosTree/LosTreeSitterDocument/LosTreeSitterDocument.h"
#include "core/LosTree/LosTreeSitterFoldRange/LosTreeSitterFoldRange.h"
#include "core/LosTree/LosTreeSitterNodeInfo/LosTreeSitterNodeInfo.h"

namespace LosCore
{
    QVector<LosTreeSitterFoldRange> LosTreeSitterFoldingProvider::collect(const LosTreeSitterDocument &document) const
    {
        QVector<LosTreeSitterFoldRange> ranges;
        document.visitNodes(
            [&ranges](const LosTreeSitterNodeInfo &node)
            {
                const QString compoundStatementType = QString::fromLatin1(LosCommon::LosTreeSitterNodeInfo_Constants::NODEINFO_COMPOUND_STATEMENT);

                if (node.L_type != compoundStatementType)
                {
                    return;
                }
                if (node.L_endLine <= node.L_startLine)
                {
                    return;
                }
                LosTreeSitterFoldRange range;
                range.L_startLine = node.L_startLine;
                range.L_endLine   = node.L_endLine;
                range.L_kind = node.L_type;
                if (range.isValid())
                {
                    ranges.append(range);
                }
            });
        return ranges;
    }
} // namespace LosCore