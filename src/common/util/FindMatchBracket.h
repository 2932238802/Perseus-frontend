#pragma once

#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "common/constants/ConstantsClass/LosUtilClass.h"
#include <QChar>
#include <QTextDocument>

namespace LosCommon
{
    inline QChar GetMatchingBracket(QChar ch);
    /**
     * @brief FindMatchingBracket
     *
     * @param doc
     * @param start_pos
     * @param open
     * @param close
     * @param direction
     * @return int
     */
    inline int FindMatchingBracket(const QTextDocument &doc, int start_pos, QChar open, int direction)
    {
        constexpr int MAX_SCAN = 5000;
        const int docLen       = doc.characterCount();
        int depth              = 0;
        int scanned            = 0;
        int pos                = start_pos + direction;
        QChar close            = GetMatchingBracket(open);
        while (pos >= 0 && pos < docLen && scanned < MAX_SCAN)
        {

            QChar ch = doc.characterAt(pos);
            if (ch == open)
            {
                ++depth;
            }
            else if (ch == close)
            {
                if (depth == 0)
                {
                    return pos;
                }
                else
                {
                    --depth;
                }
            }
            pos += direction;
            ++scanned;
        }
        return -1;
    }

    inline bool IsLeftBrack(QChar cha)
    {
        return cha == '(' || cha == '[' || cha == '{';
    }

    inline bool IsRightBrack(QChar cha)
    {
        return cha == ')' || cha == ']' || cha == '}';
    }

    inline QChar GetMatchingBracket(QChar ch)
    {
        switch (ch.toLatin1())
        {
        case '(':
            return ')';
        case ')':
            return '(';
        case '{':
            return '}';
        case '}':
            return '{';
        case '[':
            return ']';
        case ']':
            return '[';
        default:
            return QChar();
        }
    }



} // namespace LosCommon