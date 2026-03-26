#include "Perseus.h"
#include "./ui_Perseus.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosLsp/LosLspManager/LosLspManager.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/log/LosLog/LosLog.h"
#include "view/LosEditorTabUi/LosEditorTabUi.h"
#include "view/LosIssuesUi/LosIssuesUi.h"

/**
构造
*/
Perseus::Perseus(QWidget *parent) : QMainWindow(parent), ui(new Ui::Perseus)
{
    ui->setupUi(this);
    initConnect();
    initStyle();
    initShotcut();
}

/**
析构
*/
Perseus::~Perseus()
{
    delete ui;
}

/**
esc 关闭程序
*/
void Perseus::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
    {
        QApplication::exit(0);
    }
    QMainWindow::keyPressEvent(e); // 补充
}

/**
文件 加载完毕
*/
void Perseus::OnFileLoaded(bool isc)
{
    if (isc)
    {
        // 左侧的文件树
        QString curPath = LOS_projectFilepath.getFilePath();
        if (LOS_treeModel != nullptr)
        {
            delete LOS_treeModel;
            LOS_treeModel = nullptr;
        }
        if (LOS_rootNode != nullptr)
        {
            delete LOS_rootNode;
        }
        LOS_rootNode = nullptr;

        LOS_rootNode = LosModel::LosFileNode::create(curPath, nullptr);
        LosModel::LosFileNode::build(LOS_rootNode, curPath);
        LOS_treeModel = new LosModel::LosFileTreeModel(LOS_rootNode, nullptr);
        updateExplorer();
        QMessageBox::information(this, "info", "load project suc:" + curPath);
    }
    else
    {
        QMessageBox::critical(this, "error", "load file failed!");
    }
}

/**
文件按钮 被点击
*/
void Perseus::onFilesBtnClicked()
{
    QFileDialog dialog(this, "choose file/filefolder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::DontUseNativeDialog, false);
    // 只能显示 文件夹
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setAcceptMode(
        // 要么是打开 要么就是保存的形式
        QFileDialog::AcceptOpen);
    if (dialog.exec() == QDialog::Accepted)
    {
        LOS_projectFilepath.loadFile(dialog.selectedFiles().first());
        bool isSuc = LOS_projectFilepath.isExist();
        this->OnFileLoaded(isSuc);
    }
}

/**
双击文件
- 修复 展开的问题
- 默认已经有 展开的问题
*/
void Perseus::onExplorerFileDoubleClicked(const QModelIndex &index)
{
    if (!LOS_treeModel)
        return;
    LosModel::LosFileNode *fileNode = LOS_treeModel->nodeFromIndex(index);
    if (!fileNode)
        return;
    LOS_tabUi->openFile(fileNode->getFilePath());
}

/**
点击运行按钮
*/
void Perseus::onRunSingleFileBtnClicked()
{
    auto curWidget = LOS_tabUi->getCurEditor();
    if (!curWidget)
    {
        ERR("internal editor error ...", "Perseus");
        return;
    }
    auto curPath = LOS_tabUi->getCurFilePath();
    LOS_tabUi->saveTab();
    ui->bottom_tabwidget->setCurrentIndex(LosCommon::BottomTabWidget::OUTPUT);
    ui->output_plaintextedit->clear();
    INF("starting compilation ...", "Perseus");
    LOS_runMgr->execute(curPath);
}

void Perseus::onAppendErr(const QString &str)
{
    ERR(str, "Perseus");
}

void Perseus::onAppendLog(const QString &str)
{
    INF(str, "Perseus");
}

void Perseus::onBuildOver(bool ok)
{
    QString msg(ok == true ? "suc!" : "failed!");
    INF("build" + msg, "Perseus");
}



void Perseus::onLog(const QString &log)
{
    ui->output_plaintextedit->appendHtml(log);
}


/**
初始化连接
*/
void Perseus::initConnect()
{
    LOS_tabUi  = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
    LOS_runMgr = new LosCore::LosRunManager(this);
    LOS_lspMgr = new LosCore::LosLspManager(this);
    LOS_lspMgr->start();
    connect(ui->files_btn, &QPushButton::clicked, this, &Perseus::onFilesBtnClicked);
    // enter 自动触发 actived
    connect(ui->explorer_treeview, &QTreeView::activated, this, &Perseus::onExplorerFileDoubleClicked);
    // connect(ui->explorer_treeview, &QTreeView::doubleClicked, this, &Perseus::onExplorerFileDoubleClicked);
    connect(ui->run_singleFile_btn, &QPushButton::clicked, this, &Perseus::onRunSingleFileBtnClicked);
    connect(&LosCore::LosLog::instance(), &LosCore::LosLog::_sendLog, this, &Perseus::onLog);
}

/**
初始化样式F
*/
void Perseus::initStyle()
{
    ui->editor_tabwidget->setTabsClosable(true);
    ui->main_splitter->setStretchFactor(0, 1);
    ui->main_splitter->setStretchFactor(1, 4);
    ui->right_splitter->setStretchFactor(0, 4);
    ui->right_splitter->setStretchFactor(1, 1);
    ui->bottom_tabwidget->setTabText(0, QString::fromUtf8(u8"output"));
    ui->bottom_tabwidget->setTabText(1, QString::fromUtf8(u8"issues"));
    ui->bottom_tabwidget->setTabText(2, QString::fromUtf8(u8"terminal"));
    INF("perseus Engine Initialized ... ", "Perseus");
    this->setStyleSheet(LosStyle::perseus_getStyle());
}

/**
快捷键
*/
void Perseus::initShotcut()
{
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::RUN_SINGLE_FILE, this, [=]() { onRunSingleFileBtnClicked(); }, "run single file");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FILE_OPEN, this, [=]() { onFilesBtnClicked(); }, "open folder");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::CODE_FORMAT, this, [=]() { emit LosCore::LosRouter::instance()._cmd_codeFormat(); },
        "format text");
}

/**
更新文件夹
*/
void Perseus::updateExplorer()
{
    ui->explorer_treeview->setModel(LOS_treeModel);
    ui->explorer_treeview->setHeaderHidden(true);
    ui->explorer_treeview->setAnimated(true);
    ui->explorer_treeview->setIndentation(15);
}
