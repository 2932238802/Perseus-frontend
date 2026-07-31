// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QRegularExpression>
#include <optional>
#include <qglobal.h>
#include <qlist.h>
#include <qtextcursor.h>
#include <qtextdocument.h>

namespace LosCore
{
    class LosFinder
    {
      public:
        static QList<QTextCursor> findAll(QTextDocument *document, const QString &text, QTextDocument::FindFlags flags = {});
        static std::optional<QTextCursor> findNext(QTextDocument *document, const QString &text, int from_position,
                                                   QTextDocument::FindFlags flags = {});
        static std::optional<QTextCursor> findPrevious(QTextDocument *document, const QString &text, int from_position,
                                                       QTextDocument::FindFlags flags = {});

        static QList<QTextCursor> findAll(QTextDocument *document, const QRegularExpression &expr);
        static std::optional<QTextCursor> findNext(QTextDocument *document, const QRegularExpression &expr, int from_position);
        static std::optional<QTextCursor> findPrevious(QTextDocument *document, const QRegularExpression &expr, int from_position);
    };
} // namespace LosCore
