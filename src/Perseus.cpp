#include "Perseus.h"
#include "./ui_Perseus.h"
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosConfig/LosConfigManager/LosConfigManager.h"
#include "core/LosLsp/LosLspManager/LosLspManager.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/log/LosLog/LosLog.h"
#include "models/LosFilePath/LosFilePath.h"
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
    QMainWindow::keyPressEvent(e);
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
            // 修复
            ui->explorer_treeview->setModel(nullptr);
            LOS_treeModel->deleteLater();
            LOS_treeModel = nullptr;
            LOS_rootNode  = nullptr;
        }
        LOS_rootNode = nullptr;
        LOS_rootNode = LosModel::LosFileNode::create(curPath, nullptr);
        LosModel::LosFileNode::build(LOS_rootNode, curPath);
        LOS_treeModel = new LosModel::LosFileTreeModel(LOS_rootNode, nullptr);
        ui->explorer_treeview->updateExplorer(LOS_treeModel);
        INF("load project suc:" + curPath, "Perseus");
        LOS_configMgr->create(curPath);
        LOS_configMgr->analyse(curPath);
    }
    else
    {
        ERR("load file failed!", "Perseus");
    }
}



/**
文件按钮 被点击
支持导入文件和文件夹
*/
void Perseus::onFilesBtnClicked()
{
    QFileDialog dialog(this, "choose file/filefolder");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setOption(QFileDialog::DontUseNativeDialog, false);
    // 只能显示 文件夹
    dialog.setAcceptMode(
        // 要么是打开 要么就是保存的形式
        QFileDialog::AcceptOpen);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString filePathChoose = dialog.selectedFiles().first();
        LosModel::LosFilePath path(filePathChoose);
        if (path.isFile())
        {
            QString projectDirOfPath = path.getAbsolutePath();
            LOS_projectFilepath.loadFile(projectDirOfPath);
            bool isSuc = LOS_projectFilepath.isExist();
            this->OnFileLoaded(isSuc);
        }
        else if (path.isDir())
        {
            LOS_projectFilepath.loadFile(filePathChoose);
            bool isSuc = LOS_projectFilepath.isExist();
            this->OnFileLoaded(isSuc);
        }
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



/**
 */
void Perseus::onAppendErr(const QString &str)
{
    ERR(str, "Perseus");
}



void Perseus::onAppendLog(const QString &str)
{
    INF(str, "Perseus");
}



/**
建立完毕
*/
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
    LOS_tabUi     = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
    LOS_runMgr    = new LosCore::LosRunManager(this);
    LOS_lspMgr    = new LosCore::LosLspManager(this);
    LOS_configMgr = new LosCore::LosConfigManager(this);
    LOS_lspMgr->start();
    connect(ui->files_btn, &QPushButton::clicked, this, &Perseus::onFilesBtnClicked);
    // enter 自动触发 actived
    connect(ui->explorer_treeview, &QTreeView::activated, this, &Perseus::onExplorerFileDoubleClicked);
    // connect(ui->explorer_treeview, &QTreeView::doubleClicked, this, &Perseus::onExplorerFileDoubleClicked);
    connect(ui->run_singleFile_btn, &QPushButton::clicked, this, &Perseus::onRunSingleFileBtnClicked);
    connect(&LosCore::LosLog::instance(), &LosCore::LosLog::_sendLog, this, &Perseus::onLog);
    connect(
        &LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_fileSystemChanged, this,
        [=]() { OnFileLoaded(true); }, Qt::QueuedConnection);
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
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FILE_SAVE, this,
        [=]()
        {
            if (LOS_tabUi)
                LOS_tabUi->saveTab();
            if (LOS_configMgr->isInFiles(LosModel::LosFilePath(LOS_tabUi->getCurFilePath()).getBaseFileName()))
            {
                INF("rebuild..", "Perseus");
                emit LosCore::LosRouter::instance()._cmd_lsp_msg_didChangeWatchedFiles(
                    LOS_tabUi -> getCurFilePath(),
                    LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Changed);
            };
        },
        "run single file");
}
