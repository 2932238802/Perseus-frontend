
#pragma once
#include "models/LosFileNode/LosFileNode.h"
#include <QAbstractItemModel>
#include <QFileIconProvider>
#include <QObject>
#include <qabstractitemmodel.h>
#include <qtmetamacros.h>

namespace LosModel
{
class LosFileTreeModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    explicit LosFileTreeModel(LosFileNode *rootNode, QObject *parent = nullptr);

    ~LosFileTreeModel() override;

    // 问行数
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    // 找爸爸
    QModelIndex parent(const QModelIndex &child) const override;
    
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // 问行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // 问列数
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // 要数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    LosFileNode *nodeFromIndex(const QModelIndex &index);

  private:
    LosModel::LosFileNode *LOS_rootNode;
};

} // namespace LosModel
