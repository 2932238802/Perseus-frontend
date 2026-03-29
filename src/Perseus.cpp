#include "Perseus.h"
#include "./ui_Perseus.h"
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosToolMissUi/LosToolMissUi.h"

/**
构造
*/
Perseus::Perseus(QWidget *parent) : QMainWindow(parent), ui(new Ui::Perseus)
{
    ui->setupUi(this);
    initConnect();
    initStyle();
    initShotcut();
    // 等待 100 毫秒后，
    // 调用当前对象的 initSession () 函数
    // 只执行一次执行完就结束
    QTimer::singleShot(LosCommon::Perseus_Constants::WAIT_FOR_SESSION_TIME_MS, this, &Perseus::initSession);
}

/**
析构
*/
Perseus::~Perseus()
{
    LosCore::LosSession::instance().saveConfig(collectConfig());
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
        LosModel::LosFilePath projectPath = LosCore::LosState::instance().get<LosModel::LosFilePath>(
            LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);
        if (!projectPath.isExist())
        {
            ERR("project path does not exist in global state!", "Perseus");
            return;
        }
        QString curPath{projectPath.getFilePath()};
        if (LOS_treeModel != nullptr)
        {
            ui->explorer_treeview->setModel(nullptr);
            LOS_treeModel->deleteLater();
            LOS_treeModel = nullptr;
            LOS_rootNode  = nullptr;
        }
        LOS_rootNode = nullptr;
        LOS_rootNode = LosModel::LosFileNode::create(curPath, nullptr);
        LosModel::LosFileNode::build(LOS_rootNode, curPath,
                                     [this, curPath]()
                                     {
                                         LOS_treeModel = new LosModel::LosFileTreeModel(LOS_rootNode, nullptr);
                                         ui->explorer_treeview->updateExplorer(LOS_treeModel);
                                         INF("load project suc:" + curPath, "Perseus");
                                         LOS_configMgr->create(curPath);
                                         LOS_configMgr->analyse(curPath);
                                         emit LosCore::LosRouter::instance()._cmd_fileTreeDone();
                                     });
    }
    else
    {
        ERR("load file failed!", "Perseus");
    }
}



/**
- 文件按钮 被点击
- 支持导入文件和文件夹
- 修复 相关问题
- 仅仅 支持 文件选择 默认 生成 一个 文件夹
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
        QString projectDirOfPath = path.isFile() ? path.getAbsolutePath() : path.getFilePath();
        INF(projectDirOfPath, "Perseus");
        LosModel::LosFilePath projectFilepath(projectDirOfPath);
        bool isSuc = projectFilepath.isExist();
        LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR,
                                                                 projectFilepath);
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
    const auto &file = fileNode->getFile();
    if (file.isFile())
    {
        LOS_tabUi->openFile(file);
    }
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
    ui->bottom_tabwidget->setCurrentIndex(LosCommon::Perseus_Constants::BottomTabWidget::OUTPUT);
    ui->output_plaintextedit->clear();
    INF("starting compilation ...", "Perseus");
    LOS_runMgr->execute(curPath, L_project);
}



/**
- 项目 按钮的点击
*/
void Perseus::onProjectBtnClicked(bool checked)
{
    L_project = checked;
}



/**
- 打印日志
*/
void Perseus::onLog(const QString &log)
{
    ui->output_plaintextedit->appendHtml(log);
}


/**
- 字体缩放实现
- this->setStyleSheet(this->styleSheet()); 增加对应的 格式 修复

*/
void Perseus::onZoomUi(int delta)
{
    QFont font  = QApplication::font();
    int newSize = font.pointSize() + delta;
    if (newSize < LosCommon::Perseus_Constants::ZOOM_MIN)
        newSize = LosCommon::Perseus_Constants::ZOOM_MIN;
    if (newSize > LosCommon::Perseus_Constants::ZOOM_MAX)
        newSize = LosCommon::Perseus_Constants::ZOOM_MAX;
    font.setPointSize(newSize);
    QApplication::setFont(font);
    this->setStyleSheet(this->styleSheet());
}


void Perseus::onToolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
{
    LosView::LosToolMissUi dialog(config, this);
    dialog.exec();
}


/**
初始化连接
*/
void Perseus::initConnect()
{
    connect(&LosCore::LosLog::instance(), &LosCore::LosLog::_sendLog, this, &Perseus::onLog);

    LOS_tabUi        = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
    LOS_runMgr       = new LosCore::LosRunManager(this);
    LOS_lspMgr       = new LosCore::LosLspManager(this);
    LOS_configMgr    = new LosCore::LosConfigManager(this);
    LOS_toolChainMgr = new LosCore::LosToolChainManager(this);
    connect(ui->files_btn, &QPushButton::clicked, this, &Perseus::onFilesBtnClicked);
    // enter 自动触发 actived
    connect(ui->explorer_treeview, &QTreeView::activated, this, &Perseus::onExplorerFileDoubleClicked);
    connect(ui->run_singleFile_btn, &QPushButton::clicked, this, &Perseus::onRunSingleFileBtnClicked);
    connect(
        &LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_fileSystemChanged, this,
        [=]() { OnFileLoaded(true); }, Qt::QueuedConnection);
    connect(ui->project_btn, &QRadioButton::toggled, this, &Perseus::onProjectBtnClicked);

    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_toolChainMissing, this,
            &Perseus::onToolChainMissing);
}

/**
初始化样式F
*/
void Perseus::initStyle()
{
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(11);
    QApplication::setFont(defaultFont);
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
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FONT_ZOOM_IN, this,
        [=]()
        {
            INF("larger...", "Perseus");
            this->onZoomUi(2);
        },
        "zoom in");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FONT_ZOOM_OUT, this,
        [=]()
        {
            INF("smaller...", "Perseus");
            this->onZoomUi(-2);
        },
        "zoom out");
}



/**
- 初始化会话
*/
void Perseus::initSession()
{
    LosCommon::LosSession_Constants::Config conf;
    if (!LosCore::LosSession::instance().loadConfig(&conf))
        return;
    LosModel::LosFilePath file(conf.L_curProDir);
    bool isSuc = file.isExist();
    LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, file);
    OnFileLoaded(isSuc);
    if (!LOS_tabUi)
        return;

    connect(
        &LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_fileTreeDone, this,
        [=]()
        {
            for (const auto &file : conf.L_curFilePaths)
            {
                LOS_tabUi->openFile(file);
            }
            if (conf.L_curProDir.isEmpty())
                return;
            ui->explorer_treeview->expandToFile(conf.L_curFilePaths.first());
        },
        Qt::SingleShotConnection);
}



/**
- 收集当前的配置
*/
LosCommon::LosSession_Constants::Config Perseus::collectConfig()
{
    LosCommon::LosSession_Constants::Config conf;
    for (const auto &path : LOS_tabUi->getOpenFiles())
    {
        conf.L_curFilePaths.append(path);
    }
    conf.L_curProDir = LosCore::LosState::instance()
                           .get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR)
                           .getFilePath();
    return conf;
}
