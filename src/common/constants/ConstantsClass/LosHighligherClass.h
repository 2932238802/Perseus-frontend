
#pragma once
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QTextCharFormat>

namespace LosCommon
{
    namespace LosHighligher_Constants
    {
        struct SemanticToken
        {
            int L_startChar;
            int L_length;
            int L_tokenType;
            int L_tokenModifiers;
        };


        struct HighlightRule
        {
            QTextCharFormat L_format;
            QRegularExpression L_regex;
        };
    } /* namespace LosHighligher_Constants */
} // namespace LosCommon