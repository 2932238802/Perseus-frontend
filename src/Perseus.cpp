#include "Perseus.h"
#include "./ui_Perseus.h"
#include <qdebug.h>
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qshortcut.h>

/**
构造
*/
Perseus::Perseus(QWidget *parent) : QMainWindow(parent), ui(new Ui::Perseus) {
  ui->setupUi(this);
  initConnect();
  initStyle();
  initShotcut();
}

/**
析构
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
  // 只能显示 文件夹
  dialog.setOption(QFileDialog::ShowDirsOnly, true);
  dialog.setAcceptMode(
      // 要么是打开 要么就是保存的形式
      QFileDialog::AcceptOpen);
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
点击运行按钮
*/
void Perseus::onRunSingleFileBtnClicked() {
  auto curWidget = LOS_tabUi->getCurEditor();
  if (!curWidget) {
    ui->output_plaintextedit->appendPlainText(
        u8"[error]> internal editor error ...");
    return;
  }
  auto curPath = LOS_tabUi->getCurFilePath();
  LOS_tabUi->saveTab();
  ui->bottom_tabwidget->setCurrentIndex(LosCommon::BottomTabWidget::OUTPUT);
  ui->output_plaintextedit->clear();
  ui->output_plaintextedit->appendPlainText(
      u8"[info]> starting compilation ...");
  LOS_runMgr->execute(curPath);
}

void Perseus::onAppendErr(const QString &str) {
  ui->output_plaintextedit->appendPlainText(u8"[error]>" + str);
}

void Perseus::onAppendLog(const QString &str) {
  ui->output_plaintextedit->appendPlainText(u8"[info]>" + str);
}

void Perseus::onBuildOver(bool ok) {
  QString msg(ok == true ? "suc!" : "failed!");
  ui->output_plaintextedit->appendPlainText(u8"build " + msg);
}

/**
初始化连接
*/
void Perseus::initConnect() {
  LOS_tabUi = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
  LOS_runMgr = new LosCore::LosRunManager(this);
  connect(ui->files_btn, &QPushButton::clicked, this,
          &Perseus::onFilesBtnClicked);
  connect(ui->explorer_treeview, &QTreeView::doubleClicked, this,
          &Perseus::onExplorerFileDoubleClicked);
  connect(ui->run_singleFile_btn, &QPushButton::clicked, this,
          &Perseus::onRunSingleFileBtnClicked);
  connect(LOS_runMgr, &LosCore::LosRunManager::_appendErr, this,
          &Perseus::onAppendErr);
  connect(LOS_runMgr, &LosCore::LosRunManager::_appendLog, this,
          &Perseus::onAppendLog);
  connect(LOS_runMgr, &LosCore::LosRunManager::_buildOver, this,
          &Perseus::onBuildOver);
}

/**
初始化样式F
*/
void Perseus::initStyle() {
  ui->editor_tabwidget->setTabsClosable(true);
  ui->main_splitter->setStretchFactor(0, 1);
  ui->main_splitter->setStretchFactor(1, 4);
  ui->right_splitter->setStretchFactor(0, 4);
  ui->right_splitter->setStretchFactor(1, 1);
  ui->bottom_tabwidget->setTabText(0, QString::fromUtf8(u8"output"));
  ui->bottom_tabwidget->setTabText(1, QString::fromUtf8(u8"issues"));
  ui->bottom_tabwidget->setTabText(2, QString::fromUtf8(u8"terminal"));
  ui->output_plaintextedit->appendPlainText(
      u8"[info]>perseus Engine Initialized ... ");
  ui->output_plaintextedit->appendPlainText(u8"[info]> ... ");
  this->setStyleSheet(LosStyle::perseus_getStyle());
}

/**
快捷键
*/
void Perseus::initShotcut() {
  QShortcut *shortcutSave = new QShortcut(QKeySequence::Save, this);
  shortcutSave->setContext(Qt::WindowShortcut);
  QShortcut* ctrl_f5 = new QShortcut(QKeySequence(Qt::CTRL| Qt::Key_F5),this);
  connect(shortcutSave, &QShortcut::activated, this, [=]() {
    if (LOS_tabUi) {
      LOS_tabUi->saveTab();
    }
  });
  connect(ctrl_f5,&QShortcut::activated,this,&Perseus::onRunSingleFileBtnClicked);
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
