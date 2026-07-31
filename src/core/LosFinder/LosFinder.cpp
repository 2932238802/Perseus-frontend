
#include "LosFinder.h"
#include "core/LosLog/LosLog.h"
#include <optional>
#include <qtextcursor.h>

namespace LosCore
{

    /**
     * @brief 查找 所有的 匹配
     *
     * @param document
     * @param text
     * @param flags
     * @return QList<QTextCursor>
     */
    QList<QTextCursor> LosFinder::findAll(QTextDocument *document, const QString &text, QTextDocument::FindFlags flags)
    {
        QList<QTextCursor> matches;
        if (document == nullptr || text.isEmpty())
        {
            ERR("document is nullptr or match word is empty!", "LosFinder::findAll");
            return matches;
        }
        QTextCursor cursor(document);
        cursor.setPosition(0); // 从头开始
        while (true)
        {
            QTextCursor match = document->find(text, cursor, flags);
            if (match.isNull())
            {
                break;
            }
            matches.append(match);
            const int nextPosition = match.selectionEnd();
            if (nextPosition <= cursor.position())
            {
                break; // 返回了
            }
            cursor.setPosition(nextPosition);
        }
        return matches;
    }



    /**
     * @brief 往后找
     * 
     * @param document 
     * @param text 
     * @param from_position 
     * @param flags 
     * @return std::optional<QTextCursor> 
     */
    std::optional<QTextCursor> LosFinder::findNext(QTextDocument *document, const QString &text, int from_position, QTextDocument::FindFlags flags)
    {
        if (document == nullptr || text.isEmpty())
        {
            ERR("document is nullptr or match word is empty!", "LosFinder::findAll");
            return std::nullopt;
        }
        const int endPosition   = qMax(0, document->characterCount() - 1);
        const int startPosition = qBound(0, from_position, endPosition);
        QTextCursor cursor(document);
        cursor.setPosition(startPosition);
        QTextCursor match = document->find(text, cursor, flags);
        if (match.isNull() && startPosition > 0)
        {
            cursor.setPosition(0);
            match = document->find(text, cursor, flags);
        }
        if (match.isNull())
        {
            return std::nullopt;
        }
        return match;
    }



    /**
     * @brief 往前找
     * 
     * @param document 
     * @param text 
     * @param from_position 
     * @param flags 
     * @return std::optional<QTextCursor> 
     */
    std::optional<QTextCursor> LosFinder::findPrevious(QTextDocument *document, const QString &text, int from_position,
                                                       QTextDocument::FindFlags flags)
    {
        if (document == nullptr || text.isEmpty())
        {
            ERR("document is nullptr or match word is empty!", "LosFinder::findAll");
            return std::nullopt;
        }
        // 加上 往后找的逻辑
        flags |= QTextDocument::FindBackward;
        const int lastPosition  = qMax(0, document->characterCount() - 1);
        const int startPosition = qBound(0, from_position, lastPosition);
        QTextCursor cursor(document);
        cursor.setPosition(startPosition);
        QTextCursor match = document->find(text, cursor, flags);
        if (match.isNull() && startPosition < lastPosition)
        {
            cursor.setPosition(lastPosition);
            match = document->find(text, cursor, flags);
        }
        if (match.isNull())
        {
            return std::nullopt;
        }
        return match;
    }

} // namespace LosCore