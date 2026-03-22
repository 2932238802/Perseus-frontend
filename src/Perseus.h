#pragma once
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "ui/LosEditorTabUi/LosEditorTabUi.h"
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
  void updateExplorer();

public slots:
  void OnFileLoaded(bool isc);
  void onFilesBtnClicked();
  void onExplorerFileDoubleClicked(const QModelIndex &index);

private:
  Ui::Perseus *ui;
  LosModel::LosFilePath LOS_projectFilepath;
  LosModel::LosFileNode *LOS_rootNode = nullptr;
  LosModel::LosFileTreeModel *LOS_treeModel = nullptr;
  LosView::LosEditorTabUi *LOS_tabUi = nullptr;
};
