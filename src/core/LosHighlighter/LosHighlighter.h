
#pragma once

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <qregularexpression.h>
#include <qtextdocument.h>
#include <qtextformat.h>

namespace LosCore
{

class LosHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
    // QSyntaxHighlighter 必须 绑定 QTextDocument

    struct HighlightRule
    {
        QTextCharFormat L_format;
        QRegularExpression L_regex;
    };

  public:
    explicit LosHighlighter(QTextDocument *doc);
    ~LosHighlighter() = default;

  protected:
    void highlightBlock(const QString &str) override;

  private: // init
    void initRule();

  private:
    QList<HighlightRule> L_rules;
    QTextCharFormat L_keyword;
    QTextCharFormat L_class;
    QTextCharFormat L_singleComment;
    QTextCharFormat L_multiComment;
    QTextCharFormat L_str;
    QTextCharFormat L_func;
    QRegularExpression L_commentStartExpression;
    QRegularExpression L_commentEndExpression;
};
} // namespace LosCore