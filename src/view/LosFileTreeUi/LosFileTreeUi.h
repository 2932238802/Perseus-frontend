// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"

#include <QPoint>
#include <QTreeView>
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qset.h>

class QKeyEvent;

namespace LosView
{
    class LosFileTreeUi : public QTreeView
    {
        Q_OBJECT
      public:
        explicit LosFileTreeUi(QWidget *parent = nullptr);
        ~LosFileTreeUi() = default;

      public: /* tool */
        bool updateExplorer(LosModel::LosFileTreeModel *);
        void copyFileOrFolder(const QString &, const QString &);
        void deleteFileOrFolder(const QString &);
        bool expandToFile(const QString &);
        bool restoreExpandedState(const QSet<QString> &, const QString &selected_path);

      public: /* get */
        bool getExpandedPaths(QSet<QString> *get_paths);

      public slots: /* chs */
        void onCustomContextMenu(const QPoint &pos);

      protected: /* override */
        void keyPressEvent(QKeyEvent *key) override;

      private:
        void initConnect();
        void initStyle();

      private: /* innner tool */
        QModelIndex findAndExpand(LosModel::LosFileNode *, const QString &, const QModelIndex &);
    };
} /* namespace LosView */