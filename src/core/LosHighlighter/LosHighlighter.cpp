#include "core/LosHighlighter/LosHighlighter.h"

namespace LosCore
{
    LosHighlighter::LosHighlighter(QTextDocument *doc) : QSyntaxHighlighter{doc}
    {
        initRule();
    }

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
                        /*
                         * 解析只读或静态的修饰符并设置为斜体
                         */
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



    /*
     * - 合并属性
     */
    void LosHighlighter::mergeFormat(int start, int length, const QTextCharFormat &format)
    {
        QTextCursor cursor(document());
        cursor.setPosition(currentBlock().position() + start);
        cursor.setPosition(currentBlock().position() + start + length, QTextCursor::KeepAnchor);

        QTextCharFormat mergedFormat = cursor.charFormat();

        /*
         * 只合并设置了的属性，
         * 不覆盖未设置的
         */
        if (format.hasProperty(QTextFormat::ForegroundBrush))
        {
            mergedFormat.setForeground(format.foreground());
        }
        if (format.hasProperty(QTextFormat::FontItalic))
        {
            mergedFormat.setFontItalic(format.fontItalic());
        }
        if (format.hasProperty(QTextFormat::FontWeight))
        {
            mergedFormat.setFontWeight(format.fontWeight());
        }
        cursor.setCharFormat(mergedFormat);
    }



    /*
     * highlightByRegex
     * - 通过 正则 匹配颜色
     */
    void LosHighlighter::highlightByRegex(const QString &str)
    {
        /*
         * 传入的就是当前行的内容
         */
        for (const LosCommon::LosHighligher_Constants::HighlightRule &rule : L_rules)
        {
            /*
             * 这里 匹配
             */
            QRegularExpressionMatchIterator matchIt = rule.L_regex.globalMatch(str);
            while (matchIt.hasNext())
            {
                QRegularExpressionMatch match = matchIt.next();
                /*
                 * 这里上色
                 */
                setFormat(match.capturedStart(), match.capturedLength(), rule.L_format);
            }
        }
        /*
         * 默认不在注释
         */
        setCurrentBlockState(0);
        int startIndex = 0;
        if (previousBlockState() != 1)
        {
            /*
             * 不是注释 那么 就看 这一行
             * 返回的是 起始位置 indexOf
             */
            startIndex = str.indexOf(L_commentStartExpression);
        }
        while (startIndex >= 0)
        {
            /*
             * 从 找到 左半边 注释 开始 到 右边
             */
            QRegularExpressionMatch match = L_commentEndExpression.match(str, startIndex);
            int endIndex                  = match.capturedStart();
            int commentLenth              = 0;
            if (endIndex == -1)
            {
                /*
                 * 这里 -1 就是找不到
                 * 然后 设置 当前行 为 注释行 方便 给之后的 调用
                 * 多看多看
                 */
                setCurrentBlockState(1);
                commentLenth = str.length() - startIndex;
            }
            else
            {
                /*
                 * 末尾开始位置 - 开始位置 + 1 - 1 + 长度
                 * abcdedab a = 0   a = 6
                 */
                commentLenth = endIndex - startIndex + match.capturedLength();
            }
            setFormat(startIndex, commentLenth, L_multiComment);
            // /*1*/
            startIndex = str.indexOf(L_commentStartExpression, startIndex + commentLenth);
        }
    }



    /**
     * @brief updateSemanticTokens
     *
     *
     * @param in data
     *
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

        /*
         * 重绘 这一块
         */
        rehighlight();
    }



    /*
     * - 初始化 semantic 规则
     */
    void LosHighlighter::initSemanticLegend(const QStringList &legendTokenTypes,
                                            const QStringList &legendTokenModifiers)
    {
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



    /*
     * - 初始化 绘图 规则
     */
    void LosHighlighter::initRule()
    {
        LosCommon::LosHighligher_Constants::HighlightRule rule;

        const QString keywordPatterns[] = {
            QStringLiteral("\\bchar\\b"),     QStringLiteral("\\bclass\\b"),     QStringLiteral("\\bconst\\b"),
            QStringLiteral("\\bdouble\\b"),   QStringLiteral("\\benum\\b"),      QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\bfriend\\b"),   QStringLiteral("\\binline\\b"),    QStringLiteral("\\bint\\b"),
            QStringLiteral("\\blong\\b"),     QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
            QStringLiteral("\\bprivate\\b"),  QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
            QStringLiteral("\\bshort\\b"),    QStringLiteral("\\bsignals\\b"),   QStringLiteral("\\bsigned\\b"),
            QStringLiteral("\\bslots\\b"),    QStringLiteral("\\bstatic\\b"),    QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"),   QStringLiteral("\\btypename\\b"),
            QStringLiteral("\\bunion\\b"),    QStringLiteral("\\bunsigned\\b"),  QStringLiteral("\\bvirtual\\b"),
            QStringLiteral("\\bvoid\\b"),     QStringLiteral("\\bvolatile\\b"),  QStringLiteral("\\bbool\\b"),
            QStringLiteral("\\btrue\\b"),     QStringLiteral("\\bfalse\\b"),     QStringLiteral("\\bauto\\b"),
            QStringLiteral("\\breturn\\b"),   QStringLiteral("\\busing\\b"),     QStringLiteral("\\bnew\\b"),
            QStringLiteral("\\bdelete\\b"),   QStringLiteral("\\bnullptr\\b"),   QStringLiteral("\\bif\\b"),
            QStringLiteral("\\belse\\b"),     QStringLiteral("\\bfor\\b"),       QStringLiteral("\\bwhile\\b"),
            QStringLiteral("\\bdo\\b"),       QStringLiteral("\\bswitch\\b"),    QStringLiteral("\\bcase\\b"),
            QStringLiteral("\\bbreak\\b"),    QStringLiteral("\\bcontinue\\b"),  QStringLiteral("\\bdefault\\b"),
            QStringLiteral("\\bgoto\\b"),     QStringLiteral("\\bconstexpr\\b"), QStringLiteral("\\bcatch\\b"),
            QStringLiteral("\\btry\\b"),      QStringLiteral("\\bthrow\\b")};
        L_keyword.setForeground(QColor("#ff79c6"));
        for (const auto &str : keywordPatterns)
        {
            rule.L_regex  = QRegularExpression(str);
            rule.L_format = L_keyword;
            L_rules.append(rule);
        }

        QTextCharFormat preprocessorFormat;
        preprocessorFormat.setForeground(QColor("#ff79c6"));
        rule.L_regex  = QRegularExpression(QStringLiteral("#[a-zA-Z_]+\\b"));
        rule.L_format = preprocessorFormat;
        L_rules.append(rule);

        L_class.setForeground(QColor("#8be9fd"));
        L_class.setFontItalic(true);
        rule.L_regex  = QRegularExpression(QStringLiteral("\\b[Q]?[A-Z][a-zA-Z0-9_]+\\b"));
        rule.L_format = L_class;
        L_rules.append(rule);

        L_func.setForeground(QColor("#50fa7b"));
        rule.L_regex  = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        rule.L_format = L_func;
        L_rules.append(rule);

        L_str.setForeground(QColor("#f1fa8c"));
        rule.L_regex  = QRegularExpression(QStringLiteral("\"[^\"]*\"|(?<=#include )<[^>]+>"));
        rule.L_format = L_str;
        L_rules.append(rule);

        L_str.setForeground(QColor("#f1fa8c"));
        rule.L_regex  = QRegularExpression(QStringLiteral("\"[^\"]*\""));
        rule.L_format = L_str;
        L_rules.append(rule);

        L_singleComment.setForeground(QColor("#6272a4"));
        L_singleComment.setFontItalic(true);
        rule.L_regex  = QRegularExpression(QStringLiteral("//[^\n]*"));
        rule.L_format = L_singleComment;
        L_rules.append(rule);

        L_multiComment.setForeground(QColor("#6272a4"));
        L_multiComment.setFontItalic(true);
        L_commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        L_commentEndExpression   = QRegularExpression(QStringLiteral("\\*/"));

        L_themeConfig["class"]  = L_class;
        L_themeConfig["struct"] = L_class;
        L_themeConfig["type"]   = L_class;

        L_themeConfig["function"] = L_func;
        L_themeConfig["method"]   = L_func;

        L_themeConfig["string"]  = L_str;
        L_themeConfig["keyword"] = L_keyword;
        L_themeConfig["comment"] = L_singleComment;

        QTextCharFormat varFormat;
        varFormat.setForeground(QColor("#f8f8f2"));
        L_themeConfig["variable"]  = varFormat;
        L_themeConfig["parameter"] = varFormat;
        L_themeConfig["property"]  = varFormat;

        QTextCharFormat macroFormat;
        macroFormat.setForeground(QColor("#ff79c6"));
        L_themeConfig["macro"] = macroFormat;

        QTextCharFormat enumMemberFormat;
        enumMemberFormat.setForeground(QColor("#bd93f9"));
        L_themeConfig["enumMember"] = enumMemberFormat;
    }


} /* namespace LosCore */