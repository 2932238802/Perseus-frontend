#pragma once
#include "common/constants/ConstantsNum.h"
#include "core/run/LosRunManager/LosRunManager.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "ui/LosEditorTabUi/LosEditorTabUi.h"
#include "ui/LosEditorUi/LosEditorUi.h"
#include "ui/style/perseus_style.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>

QT_BEGIN_NAMESPACE
namespace Ui {
class Perseus;
}
QT_END_NAMESPACE

class Perseus : public QMainWindow {
  Q_OBJECT

public:
  Perseus(QWidget *parent = nullptr);
  ~Perseus();

protected:
  void keyPressEvent(QKeyEvent *) override;

private:
  void initConnect();
  void initStyle();
  void initShotcut();
  void updateExplorer();

public slots:
  void OnFileLoaded(bool isc);
  void onFilesBtnClicked();
  void onExplorerFileDoubleClicked(const QModelIndex &index);
  void onRunSingleFileBtnClicked();
  void onAppendErr(const QString&);
  void onAppendLog(const QString&);
  void onBuildOver(bool);

private:
  Ui::Perseus *ui;
  LosModel::LosFilePath LOS_projectFilepath;
  LosModel::LosFileNode *LOS_rootNode = nullptr;
  LosModel::LosFileTreeModel *LOS_treeModel = nullptr;
  LosView::LosEditorTabUi *LOS_tabUi = nullptr;
  LosCore::LosRunManager* LOS_runMgr = nullptr;
};
