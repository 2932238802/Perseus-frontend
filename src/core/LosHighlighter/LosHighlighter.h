// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"

#include <QSyntaxHighlighter>
#include <memory>
#include <qtmetamacros.h>

class QJsonArray;
class QString;
class QTextCharFormat;
class QTextDocument;

namespace LosCore
{
    class LosHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT

      public:
        explicit LosHighlighter(QTextDocument *doc);
        ~LosHighlighter() override;
        void updateSemanticTokens(const QJsonArray &data);
        void initSemanticLegend(const QStringList &legendTokenTypes, const QStringList &legendTokenModifiers = QStringList());

      public: // tool
        void setLang(LosCommon::LosToolChain_Constants::LosLanguage lang);
        void setTheme(const QString &themeName);

      protected:
        void highlightBlock(const QString &str) override;

      private: // highlight
        void initRule();
        void mergeFormat(int start, int length, const QTextCharFormat &format);
        void highlightByRegex(const QString &str);

      private:
        class Impl;
        std::unique_ptr<Impl> L_highlighterImpl;
    };
} /* namespace LosCore */
