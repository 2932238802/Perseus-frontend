
#pragma once
#include "models/LosFileNode/LosFileNode.h"
#include <QAbstractItemModel>
#include <QFileIconProvider>
#include <QObject>
#include <qabstractitemmodel.h>
#include <qtmetamacros.h>
#include <qvariant.h>

namespace LosModel
{
    class LosFileTreeModel : public QAbstractItemModel
    {
        Q_OBJECT
      public:
        explicit LosFileTreeModel(LosFileNode *rootNode, QObject *parent = nullptr);
        ~LosFileTreeModel() override;


      public:                                                                                             /* override */
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override; /* 问行数 */
        QModelIndex parent(const QModelIndex &child) const override; /* 找爸爸 */
        bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;             /* 问行数 */
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;          /* 问列数 */
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override; /* 要数据 */
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

      public: /* tool */
        LosFileNode *nodeFromIndex(const QModelIndex &index);
        LosModel::LosFileNode *getRoot() const;

      private:
        LosModel::LosFileNode *LOS_rootNode;
    };

} /* namespace LosModel */
