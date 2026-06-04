// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "core/LosHighlighter/LosHighlighter.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"

#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCursor>

namespace LosCore
{
    /**
     * @brief Construct a new Los Highlighter:: Los Highlighter object
     * 
     * @param doc 
     */
    LosHighlighter::LosHighlighter(QTextDocument *doc) : QSyntaxHighlighter{doc}
    {
        initRule();
    }



    /**
     * @brief highlightBlock
     *
     * @param str
     */
    void LosHighlighter::highlightBlock(const QString &str)
    {
        highlightByRegex(str);
        int currentLine = currentBlock().blockNumber();
        if (L_semanticData.contains(currentLine))
        {
            for (const LosCommon::LosHighligher_Constants::SemanticToken &token : L_semanticData[currentLine])
            {
                if (token.L_tokenType < L_semanticFormats.size())
                {
                    QTextCharFormat format = L_semanticFormats.at(token.L_tokenType);
                    if (format.isValid())
                    {
                        if (L_readonlyModifierIndex != -1 && (token.L_tokenModifiers & (1 << L_readonlyModifierIndex)))
                        {
                            format.setFontItalic(true);
                        }
                        if (L_staticModifierIndex != -1 && (token.L_tokenModifiers & (1 << L_staticModifierIndex)))
                        {
                            format.setFontItalic(true);
                        }
                        mergeFormat(token.L_startChar, token.L_length, format);
                    }
                }
            }
        }
    }



    /**
     * @brief mergeFormat
     *
     * @param start
     * @param length
     * @param format
     */
    void LosHighlighter::mergeFormat(int start, int length, const QTextCharFormat &format)
    {
        QTextCursor cursor(document());
        cursor.setPosition(currentBlock().position() + start);
        cursor.setPosition(currentBlock().position() + start + length, QTextCursor::KeepAnchor);
        QTextCharFormat mergedFormat = cursor.charFormat();
        if (format.hasProperty(QTextFormat::ForegroundBrush))
        {
            mergedFormat.setForeground(format.foreground());
        }
        if (format.hasProperty(QTextFormat::BackgroundBrush))
        {
            mergedFormat.setBackground(format.background());
        }
        if (format.hasProperty(QTextFormat::FontItalic))
        {
            mergedFormat.setFontItalic(format.fontItalic());
        }
        if (format.hasProperty(QTextFormat::FontWeight))
        {
            mergedFormat.setFontWeight(format.fontWeight());
        }
        if (format.hasProperty(QTextFormat::FontUnderline))
        {
            mergedFormat.setFontUnderline(format.fontUnderline());
        }
        cursor.setCharFormat(mergedFormat);
    }



    /**
     * @brief highlightByRegex
     * 通过 正则 匹配颜色
     * @param str
     */
    void LosHighlighter::highlightByRegex(const QString &str)
    {
        for (const LosCommon::LosHighligher_Constants::HighlightRule &rule : L_rules)
        {
            QRegularExpressionMatchIterator matchIt = rule.L_regex.globalMatch(str);
            while (matchIt.hasNext())
            {
                QRegularExpressionMatch match = matchIt.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.L_format);
            }
        }
        if (!L_hasMultiLineComment)
        {
            setCurrentBlockState(0);
            return;
        }

        setCurrentBlockState(0);
        int startIndex = 0;
        if (previousBlockState() != 1)
        {
            startIndex = str.indexOf(L_commentStartExpression);
        }
        while (startIndex >= 0)
        {
            QRegularExpressionMatch match = L_commentEndExpression.match(str, startIndex);
            int endIndex                  = match.capturedStart();
            int commentLenth              = 0;
            if (endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLenth = str.length() - startIndex;
            }
            else
            {
                commentLenth = endIndex - startIndex + match.capturedLength();
            }
            setFormat(startIndex, commentLenth, L_multiComment);
            startIndex = str.indexOf(L_commentStartExpression, startIndex + commentLenth);
        }
    }



    /**
     * @brief updateSemanticTokens
     * @param in data
     * 更新一下
     * - [deltaLine, deltaChar, length, tokenType, modifiers]
     * - 分别是五个
     */
    void LosHighlighter::updateSemanticTokens(const QJsonArray &data)
    {
        L_semanticData.clear();
        int currentLine = 0;
        int currentChar = 0;
        for (int i = 0; i + 4 < data.size(); i += 5)
        {
            int deltaLine = data[i].toInt();
            int deltaChar = data[i + 1].toInt();
            int length    = data[i + 2].toInt();
            int tokenType = data[i + 3].toInt();
            int modifiers = data[i + 4].toInt();
            currentLine += deltaLine;
            if (deltaLine > 0)
            {
                currentChar = deltaChar;
            }
            else
            {
                currentChar += deltaChar;
            }
            L_semanticData[currentLine].append({currentChar, length, tokenType, modifiers});
        }
        rehighlight();
    }



    /**
     * @brief initSemanticLegend
     * 
     * @param legendTokenTypes 
     * @param legendTokenModifiers 
     */
    void LosHighlighter::initSemanticLegend(const QStringList &legendTokenTypes, const QStringList &legendTokenModifiers)
    {
        L_legendTokenTypes     = legendTokenTypes;
        L_legendTokenModifiers = legendTokenModifiers;
        L_semanticFormats.clear();
        L_semanticFormats.resize(legendTokenTypes.size());
        for (int i = 0; i < legendTokenTypes.size(); ++i)
        {
            QString tokenName = legendTokenTypes.at(i);
            if (L_themeConfig.contains(tokenName))  
            {
                L_semanticFormats[i] = L_themeConfig.value(tokenName);
            }
            else
            {
                L_semanticFormats[i] = QTextCharFormat();
            }
        }
        L_readonlyModifierIndex = legendTokenModifiers.indexOf("readonly");
        L_staticModifierIndex   = legendTokenModifiers.indexOf("static");
    }



    /**
     * @brief setLang 设置语言
     * @param lang
     */
    void LosHighlighter::setLang(LosCommon::LosToolChain_Constants::LosLanguage lang)
    {
        if (L_curLang == lang)
        {
            return;
        }
        L_curLang = lang;
        initRule();
        rehighlight();
    }



    /**
     * @brief setTheme 设置高亮主题
     * @param themeName 主题名，例如 dracula / light / nord / monokai
     */
    void LosHighlighter::setTheme(const QString &themeName)
    {
        if (L_curThemeName == themeName)
        {
            return;
        }
        L_curThemeName = themeName;
        initRule();
        rehighlight();
    }



    /**
     * @brief initRule
     */
    void LosHighlighter::initRule()
    {
        L_rules.clear();
        L_themeConfig.clear();
        L_multiComment = QTextCharFormat();
        L_commentStartExpression = QRegularExpression();
        L_commentEndExpression   = QRegularExpression();
        L_hasMultiLineComment    = false;
        if (!loadThemeFromJson(themeConfigPath()))
        {
            loadThemeFromJson(QStringLiteral(":/highlight/themes/dracula.json"));
        }
        loadLanguageFromJson(languageConfigPath());
        if (!L_legendTokenTypes.isEmpty())
        {
            initSemanticLegend(L_legendTokenTypes, L_legendTokenModifiers);
        }
    }



    /**
     * @brief loadThemeFromJson 从 json 中读取主题样式
     * @param themePath 主题 json 路径
     * @return 是否读取成功
     */
    bool LosHighlighter::loadThemeFromJson(const QString &themePath)
    {
        QFile file(themePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QJsonParseError error;
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError || !doc.isObject())
        {
            return false;
        }

        const QJsonObject root    = doc.object();
        const QJsonObject formats = root.value(QStringLiteral("formats")).toObject();
        if (formats.isEmpty())
        {
            return false;
        }

        for (auto it = formats.begin(); it != formats.end(); ++it)
        {
            if (!it.value().isObject())
            {
                continue;
            }
            L_themeConfig[it.key()] = parseFormat(it.value().toObject());
        }

        return true;
    }



    /**
     * @brief loadLanguageFromJson 从 json 中读取语言正则规则
     * @param languagePath 语言 json 路径
     * @return 是否读取成功
     */
    bool LosHighlighter::loadLanguageFromJson(const QString &languagePath)
    {
        QFile file(languagePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QJsonParseError error;
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error != QJsonParseError::NoError || !doc.isObject())
        {
            return false;
        }

        const QJsonObject root = doc.object();
        const QJsonArray rules = root.value(QStringLiteral("rules")).toArray();

        for (const QJsonValue &ruleValue : rules)
        {
            if (!ruleValue.isObject())
            {
                continue;
            }

            const QJsonObject ruleObject = ruleValue.toObject();
            const QString name           = ruleObject.value(QStringLiteral("name")).toString();
            const QJsonArray patterns    = ruleObject.value(QStringLiteral("patterns")).toArray();

            if (name.isEmpty() || patterns.isEmpty())
            {
                continue;
            }

            const QTextCharFormat format = L_themeConfig.value(name);

            for (const QJsonValue &patternValue : patterns)
            {
                const QString pattern = patternValue.toString();
                if (pattern.isEmpty())
                {
                    continue;
                }

                LosCommon::LosHighligher_Constants::HighlightRule rule;
                rule.L_regex = QRegularExpression(pattern);
                if (!rule.L_regex.isValid())
                {
                    continue;
                }

                rule.L_format = format;
                L_rules.append(rule);
            }
        }

        const QJsonObject multiLineComment = root.value(QStringLiteral("multiLineComment")).toObject();
        const QString commentName          = multiLineComment.value(QStringLiteral("name")).toString();
        const QString startPattern         = multiLineComment.value(QStringLiteral("start")).toString();
        const QString endPattern           = multiLineComment.value(QStringLiteral("end")).toString();

        if (!commentName.isEmpty() && !startPattern.isEmpty() && !endPattern.isEmpty())
        {
            L_commentStartExpression = QRegularExpression(startPattern);
            L_commentEndExpression   = QRegularExpression(endPattern);
            L_multiComment           = L_themeConfig.value(commentName);
            L_hasMultiLineComment    = L_commentStartExpression.isValid() && L_commentEndExpression.isValid();
        }

        return true;
    }



    /**
     * @brief parseFormat 把 json 样式转换成 QTextCharFormat
     * @param obj json 样式对象
     * @return QTextCharFormat
     */
    QTextCharFormat LosHighlighter::parseFormat(const QJsonObject &obj) const
    {
        QTextCharFormat format;

        if (obj.contains(QStringLiteral("foreground")))
        {
            format.setForeground(QColor(obj.value(QStringLiteral("foreground")).toString()));
        }

        if (obj.contains(QStringLiteral("background")))
        {
            format.setBackground(QColor(obj.value(QStringLiteral("background")).toString()));
        }

        if (obj.value(QStringLiteral("italic")).toBool(false))
        {
            format.setFontItalic(true);
        }

        if (obj.value(QStringLiteral("bold")).toBool(false))
        {
            format.setFontWeight(QFont::Bold);
        }

        if (obj.value(QStringLiteral("underline")).toBool(false))
        {
            format.setFontUnderline(true);
        }

        return format;
    }



    /**
     * @brief languageConfigPath 获取当前语言配置路径
     * @return 语言 json 路径
     */
    QString LosHighlighter::languageConfigPath() const
    {
        if (L_curLang == LosCommon::LosToolChain_Constants::LosLanguage::PYTHON)
        {
            return QStringLiteral(":/highlight/languages/python.json");
        }

        return QStringLiteral(":/highlight/languages/cpp.json");
    }



    /**
     * @brief themeConfigPath 获取当前主题配置路径
     * @return 主题 json 路径
     */
    QString LosHighlighter::themeConfigPath() const
    {
        return QStringLiteral(":/highlight/themes/%1.json").arg(L_curThemeName);
    }
} /* namespace LosCore */
