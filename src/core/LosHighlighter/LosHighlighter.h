#pragma once
#include "common/constants/ConstantsClass/LosHighligherClass.h"

#include <QFont>
#include <QJsonArray>
#include <QMap>
#include <QRegularExpression>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>
#include <qnamespace.h>
#include <qtextdocument.h>
#include <qtextformat.h>

namespace LosCore
{
    class LosHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT

      public:
        explicit LosHighlighter(QTextDocument *doc);
        ~LosHighlighter() = default;
        void updateSemanticTokens(const QJsonArray &data);
        void initSemanticLegend(const QStringList &legendTokenTypes,
                                const QStringList &legendTokenModifiers = QStringList());

      protected:
        void highlightBlock(const QString &str) override;

      private: /* init */
        void mergeFormat(int start, int length, const QTextCharFormat &format);

      private: // init
        void initRule();
        void highlightByRegex(const QString &str);

      private:
        QList<LosCommon::LosHighligher_Constants::HighlightRule> L_rules;
        QTextCharFormat L_keyword;
        QTextCharFormat L_class;
        QTextCharFormat L_singleComment;
        QTextCharFormat L_multiComment;
        QTextCharFormat L_str;
        QTextCharFormat L_func;
        QRegularExpression L_commentStartExpression;
        QRegularExpression L_commentEndExpression;
        QMap<int, QList<LosCommon::LosHighligher_Constants::SemanticToken>> L_semanticData;
        QVector<QTextCharFormat> L_semanticFormats;
        QMap<QString, QTextCharFormat> L_themeConfig;
        int L_readonlyModifierIndex = -1;
        int L_staticModifierIndex   = -1;
    };
} /* namespace LosCore */
