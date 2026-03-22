#include "Perseus.h"
#include "./ui_Perseus.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"
#include <cstddef>
#include <qdialog.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qglobal.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtreeview.h>
#include <qurl.h>

/**

*/
Perseus::Perseus(QWidget *parent) : QMainWindow(parent), ui(new Ui::Perseus) {
  ui->setupUi(this);
  initConnect();
  initStyle();
}

/**


*/
Perseus::~Perseus() { delete ui; }

/**
esc 关闭程序
*/
void Perseus::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Escape) {
    QApplication::exit(0);
  }
  QMainWindow::keyPressEvent(e); // 补充
}

/**
文件 加载完毕
*/
void Perseus::OnFileLoaded(bool isc) {
  if (isc) {
    // 左侧的文件树
    QString curPath = LOS_projectFilepath.getFilePath();
    if (LOS_treeModel != nullptr) {
      delete LOS_treeModel;
      LOS_treeModel = nullptr;
    }
    if (LOS_rootNode != nullptr) {
      delete LOS_rootNode;
      LOS_rootNode = nullptr;
    }

    LOS_rootNode = LosModel::LosFileNode::create(curPath, nullptr);
    LosModel::LosFileNode::build(LOS_rootNode, curPath);
    LOS_treeModel = LosModel::LosFileTreeModel::create(LOS_rootNode, nullptr);
    updateExplorer();
    QMessageBox::information(this, "info", "load project suc:" + curPath);
  } else {
    QMessageBox::critical(this, "error", "load file failed!");
  }
}

/**
文件按钮 被点击
*/
void Perseus::onFilesBtnClicked() {
  QFileDialog dialog(this, "choose file/filefolder");
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOption(QFileDialog::DontUseNativeDialog, false);
  dialog.setOption(QFileDialog::ShowDirsOnly, true); // 只能显示 文件夹
  dialog.setAcceptMode(
      QFileDialog::AcceptOpen); // 要么是打开 要么就是保存的形式
  if (dialog.exec() == QDialog::Accepted) {
    LOS_projectFilepath.loadFile(dialog.selectedFiles().first());
    bool isSuc = LOS_projectFilepath.isExist();
    this->OnFileLoaded(isSuc);
  }
}

/**
双击文件
*/
void Perseus::onExplorerFileDoubleClicked(const QModelIndex &index) {
  if (!LOS_treeModel)
    return;
  LosModel::LosFileNode *fileNode = LOS_treeModel->nodeFromIndex(index);
  if (!fileNode)
    return;
  if (fileNode->getFileType() == LosCommon::LOS_ENUM_FileType::FT_FOLDER) {
    if (ui->explorer_treeview->isExpanded(index)) {
      ui->explorer_treeview->collapse(index);
    } else {
      ui->explorer_treeview->expand(index);
    }
    return;
  }
  LOS_tabUi->openFile(fileNode->getFilePath());
}

/**
初始化连接
*/
void Perseus::initConnect() {
  LOS_tabUi = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
  connect(ui->files_btn, &QPushButton::clicked, this,
          &Perseus::onFilesBtnClicked);
  connect(ui->explorer_treeview, &QTreeView::doubleClicked, this,
          &Perseus::onExplorerFileDoubleClicked);
}

/**
初始化样式
*/
void Perseus::initStyle() { 
    ui->editor_tabwidget->setTabsClosable(true);
    this->setStyleSheet(LosStyle::perseus_getStyle());
 }

/**
更新文件夹
*/
void Perseus::updateExplorer() {
  ui->explorer_treeview->setModel(LOS_treeModel);
  ui->explorer_treeview->setHeaderHidden(true);
  ui->explorer_treeview->setAnimated(true);
  ui->explorer_treeview->setIndentation(15);
}
