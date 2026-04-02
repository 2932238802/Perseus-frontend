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
        for (const SemanticToken &token : L_semanticData[currentLine])
        {
            if (token.tokenType < L_semanticFormats.size())
            {
                QTextCharFormat format = L_semanticFormats.at(token.tokenType);

                if (format.isValid())
                {
                    // 解析只读或静态的修饰符并设置为斜体
                    if (L_readonlyModifierIndex != -1 && (token.tokenModifiers & (1 << L_readonlyModifierIndex)))
                    {
                        format.setFontItalic(true);
                    }
                    if (L_staticModifierIndex != -1 && (token.tokenModifiers & (1 << L_staticModifierIndex)))
                    {
                        format.setFontItalic(true);
                    }

                    setFormat(token.startChar, token.length, format);
                }
            }
        }
    }
}

void LosHighlighter::highlightByRegex(const QString &str)
{
    // 传入的就是当前行的内容
    for (const HighlightRule &rule : L_rules)
    {
        // 这里 匹配
        QRegularExpressionMatchIterator matchIt = rule.L_regex.globalMatch(str);
        while (matchIt.hasNext())
        {
            QRegularExpressionMatch match = matchIt.next();
            // 这里上色
            setFormat(match.capturedStart(), match.capturedLength(), rule.L_format);
        }
    }
    // 默认不在注释
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        // 不是注释 那么 就看 这一行
        // 返回的是 起始位置 indexOf
        startIndex = str.indexOf(L_commentStartExpression);
    }
    while (startIndex >= 0)
    {
        // 从 找到 左半边 注释 开始 到 右边
        QRegularExpressionMatch match = L_commentEndExpression.match(str, startIndex);
        int endIndex                  = match.capturedStart();
        int commentLenth              = 0;
        if (endIndex == -1)
        {
            // 这里 -1 就是找不到
            // 然后 设置 当前行 为 注释行 方便 给之后的 调用
            // 多看多看
            setCurrentBlockState(1);
            commentLenth = str.length() - startIndex;
        }
        else
        {
            // 末尾开始位置 - 开始位置 + 1 - 1 + 长度
            // abcdedab a = 0   a = 6
            commentLenth = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLenth, L_multiComment);
        // /*1*/
        startIndex = str.indexOf(L_commentStartExpression, startIndex + commentLenth);
    }
}




/**
- 更新一下 
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

    // 重绘 这一块
    rehighlight();
}



void LosHighlighter::initSemanticLegend(const QStringList &legendTokenTypes, const QStringList &legendTokenModifiers)
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



/**
- 初始化 绘图 规则
*/
void LosHighlighter::initRule()
{
    HighlightRule rule;

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
        QStringLiteral("\\btrue\\b"),     QStringLiteral("\\bfalse\\b"),     QStringLiteral("\\bauto\\b")};

    L_keyword.setForeground(QColor("#E17899")); // 霓虹粉
    for (const auto &str : keywordPatterns)
    {
        rule.L_regex  = QRegularExpression(str);
        rule.L_format = L_keyword;
        L_rules.append(rule);
    }

    L_class.setForeground(QColor("#F5B83D")); 
    rule.L_regex  = QRegularExpression(QStringLiteral("\\b[Q]?[A-Z][a-zA-Z0-9_]+\\b"));
    rule.L_format = L_class;
    L_rules.append(rule);

    L_func.setForeground(QColor("#2DCCCF")); 
    rule.L_regex  = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.L_format = L_func;
    L_rules.append(rule);

    L_str.setForeground(QColor("#42E6A4")); 
    rule.L_regex  = QRegularExpression(QStringLiteral("\"[^\"]*\""));
    rule.L_format = L_str;
    L_rules.append(rule);

    L_singleComment.setForeground(QColor("#5B7888")); 
    rule.L_regex  = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.L_format = L_singleComment;
    L_rules.append(rule);

    L_multiComment.setForeground(QColor("#5B7888"));
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
    varFormat.setForeground(QColor("#C8D4F0"));
    L_themeConfig["variable"]  = varFormat;
    L_themeConfig["parameter"] = varFormat;
    L_themeConfig["property"]  = varFormat;

    QTextCharFormat macroFormat;
    macroFormat.setForeground(QColor("#B97EE6"));
    L_themeConfig["macro"]      = macroFormat;
    
    QTextCharFormat enumMemberFormat;
    enumMemberFormat.setForeground(QColor("#FF967D"));
    L_themeConfig["enumMember"] = enumMemberFormat;
}


} // namespace LosCore