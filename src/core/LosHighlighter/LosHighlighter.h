// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosHighligherClass.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QRegularExpression>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>
#include <qtextdocument.h>


namespace LosCore
{
    class LosHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT

      public:
        explicit LosHighlighter(QTextDocument *doc);
        ~LosHighlighter() = default;
        void updateSemanticTokens(const QJsonArray &data);
        void initSemanticLegend(const QStringList &legendTokenTypes, const QStringList &legendTokenModifiers = QStringList());

      public: // tool
        void setLang(LosCommon::LosToolChain_Constants::LosLanguage lang);
        void setTheme(const QString &themeName);

      protected:
        void highlightBlock(const QString &str) override;

      private: // init
        void mergeFormat(int start, int length, const QTextCharFormat &format);

      private: // init
        void initRule();
        void highlightByRegex(const QString &str);

      private: // json
        bool loadThemeFromJson(const QString &themePath);
        bool loadLanguageFromJson(const QString &languagePath);
        QTextCharFormat parseFormat(const QJsonObject &obj) const;
        QString languageConfigPath() const;
        QString themeConfigPath() const;

      private:
        LosCommon::LosToolChain_Constants::LosLanguage L_curLang = LosCommon::LosToolChain_Constants::LosLanguage::CXX;
        QString L_curThemeName = QStringLiteral("dracula");
        QList<LosCommon::LosHighligher_Constants::HighlightRule> L_rules;
        QTextCharFormat L_multiComment;
        QRegularExpression L_commentStartExpression;
        QRegularExpression L_commentEndExpression;
        QMap<int, QList<LosCommon::LosHighligher_Constants::SemanticToken>> L_semanticData;
        QVector<QTextCharFormat> L_semanticFormats;
        QMap<QString, QTextCharFormat> L_themeConfig;
        QStringList L_legendTokenTypes;
        QStringList L_legendTokenModifiers;
        bool L_hasMultiLineComment = false;
        int L_readonlyModifierIndex = -1;
        int L_staticModifierIndex   = -1;
    };
} /* namespace LosCore */
