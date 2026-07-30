// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QStyledItemDelegate>
#include <qtmetamacros.h>
namespace LosView
{

    class LosFileTreeDelegate : public QStyledItemDelegate
    {
        Q_OBJECT
      public:
        explicit LosFileTreeDelegate(QObject *parent = nullptr);

      protected: // override
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    };
} // namespace LosView