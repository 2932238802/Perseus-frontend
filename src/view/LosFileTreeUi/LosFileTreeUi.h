#pragma once
#include "core/LosFileSystem/LosFileSystem.h"
#include "core/LosLog/LosLog.h"
#include "core/LosPlatform/LosPlatform.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"

#include <QAction>
#include <QClipboard>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QObject>
#include <QPoint>
#include <QTreeView>
#include <QUrl>
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qabstractitemview.h>
#include <qdesktopservices.h>
#include <qevent.h>
#include <qguiapplication.h>
#include <qitemselectionmodel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qset.h>

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