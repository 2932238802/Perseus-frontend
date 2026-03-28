#include "core/LosHighlighter/LosHighlighter.h"


namespace LosCore
{
LosHighlighter::LosHighlighter(QTextDocument *doc) : QSyntaxHighlighter{doc}
{
    initRule();
}


void LosHighlighter::highlightBlock(const QString &str)
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

void LosHighlighter::initRule()
{
    HighlightRule rule;

    // 关键字
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

    L_keyword.setForeground(QColor("#569CD6"));
    for (const auto &str : keywordPatterns)
    {
        rule.L_regex  = QRegularExpression(str);
        rule.L_format = L_keyword;
        L_rules.append(rule);
    }

    L_class.setForeground(QColor("#4EC9B0"));
    rule.L_regex  = QRegularExpression(QStringLiteral("\\b[Q]?[A-Z][a-zA-Z0-9_]+\\b"));
    rule.L_format = L_class;
    L_rules.append(rule);

    L_func.setForeground(QColor("#DCDCAA"));
    rule.L_regex  = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.L_format = L_func;
    L_rules.append(rule);

    L_str.setForeground(QColor("#CE9178"));
    rule.L_regex  = QRegularExpression(QStringLiteral("\"[^\"]*\""));
    rule.L_format = L_str;
    L_rules.append(rule);

    L_singleComment.setForeground(QColor("#6A9955"));
    rule.L_regex  = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.L_format = L_singleComment;
    L_rules.append(rule);

    L_multiComment.setForeground(QColor("#6A9955"));
    L_commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    L_commentEndExpression   = QRegularExpression(QStringLiteral("\\*/"));
}

} // namespace LosCore