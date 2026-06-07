// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "Perseus.h"
#include "./ui_Perseus.h"
#include "core/LosGitManager/LosGitManager.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "view/LosSettingsUi/LosSettingsUi.h"


/**
 * @brief Construct a new Perseus:: Perseus object
 *
 * @param in parent
 */
Perseus::Perseus(QWidget *parent) : QMainWindow(parent), ui(new Ui::Perseus)
{
    ui->setupUi(this);
    initConnect();
    initStyle();
    initShotcut();
    /*
     * 等待 100 毫秒后，
     * 调用当前对象的 initSession () 函数
     * 只执行一次执行完就结束
     */
    QTimer::singleShot(LosCommon::Perseus_Constants::WAIT_FOR_SESSION_TIME_MS, this, &Perseus::initSession);
}



/**
 * @brief
 *
 */
Perseus::~Perseus()
{
    LosCore::LosSession::instance().saveConfig(collectConfig());
    LosCore::LosLog::instance().disconnect(this);
    if (LOS_treeModel)
        delete LOS_treeModel;
    delete ui;
}



/**
 * @brief
 *
 * @param e
 * @return * void
 *
 */
void Perseus::keyPressEvent(QKeyEvent *e)
{
    QMainWindow::keyPressEvent(e);
}



/**
 * @brief
 *
 * @param in isc
 * @param in run_analysis
 */
void Perseus::OnFileLoaded(bool isc, bool run_analysis)
{
    if (!isc)
    {
        ERR("load file failed!", "Perseus");
        return;
    }

    LosModel::LosFilePath projectPath = LosCore::LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);

    ui->toolbar_widget->getProjectDirLabel()->setText(projectPath.getFilePath());

    if (!projectPath.isExist())
    {
        ERR("project path does not exist in global state!", "Perseus");
        return;
    }

    QString curPath{projectPath.getFilePath()};
    int L_curGen      = ++L_buildGeneration;
    auto *newRootNode = LosModel::LosFileNode::create(curPath, nullptr);
    LosModel::LosFileNode::build(newRootNode, curPath,
                                 [this, curPath, newRootNode, L_curGen, run_analysis, projectPath]()
                                 {
                                     if (L_curGen != L_buildGeneration)
                                     {
                                         delete newRootNode;
                                         return;
                                     }
                                     auto oldModel = LOS_treeModel;
                                     auto oldRoot  = LOS_rootNode;
                                     LOS_rootNode  = newRootNode;
                                     LOS_treeModel = new LosModel::LosFileTreeModel(LOS_rootNode, this);
                                     ui->explorer_treeview->updateExplorer(LOS_treeModel);
                                     INF("load project suc:" + curPath, "Perseus");
                                     if (run_analysis)
                                     {
                                         LOS_configMgr->create(curPath);
                                         LOS_configMgr->analyse(curPath);
                                     }

                                     if (L_filesWatcher)
                                     {
                                         if (!L_filesWatcher->directories().isEmpty())
                                             L_filesWatcher->removePaths(L_filesWatcher->directories());
                                         L_filesWatcher->addPath(curPath);
                                         const QString buildDirPath = QDir(curPath).filePath(LosCommon::LosConfig_Constants::BUILD_NAME);
                                         QDirIterator it(curPath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
                                         while (it.hasNext())
                                         {
                                             QString dir     = it.next();
                                             QString dirName = QFileInfo(dir).fileName();
                                             /* 跳过 build/ 及其所有子目录 */
                                             if (dir == buildDirPath || dir.startsWith(buildDirPath + QDir::separator()))
                                                 continue;
                                             /* 跳过 .git 版本控制目录 */
                                             if (dirName == ".git")
                                                 continue;
                                             L_filesWatcher->addPath(dir);
                                         }
                                     }
                                     emit LosCore::LosRouter::instance()._cmd_fileTreeDone();
                                     emit LosCore::LosRouter::instance()._cmd_initGitProject(projectPath.getFilePath());
                                     if (oldModel)
                                     {
                                         oldModel->deleteLater();
                                     }
                                 });
}



/**
 * @brief onFilesBtnClicked
 * - 文件按钮 被点击
 * - 支持导入文件和文件夹
 * - 以文件夹所在的绝对位置 作为 项目根目录
 * - 切换目录时关闭其它 Tab
 *
 */
void Perseus::onFilesBtnClicked()
{
    QString pathChoose =
        QFileDialog::getExistingDirectory(this, tr("Open Project Folder"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (pathChoose.isEmpty())
    {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Select a file to open its directory"));
        if (filePath.isEmpty())
            return;
        pathChoose = QFileInfo(filePath).absolutePath();
    }
    if (!pathChoose.isEmpty())
    {
        if (LOS_tabUi)
        {
            LOS_tabUi->closeAllTabs();
        }
        LosModel::LosFilePath projectFilepath(pathChoose);
        bool isSuc = projectFilepath.isExist();
        LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, projectFilepath);
        SUC("load " + pathChoose, "Perseus");
        this->OnFileLoaded(isSuc);
    }
}



/**
 * @brief onExplorerFileDoubleClicked
 * 双击文件
 * - 修复 展开的问题
 * - 默认已经有 展开的问题
 *
 * @param index
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
 * @brief onRunSingleFileBtnClicked
 *
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
 * @brief 项目 按钮的点击
 *
 * @param checked
 */
void Perseus::onProjectBtnClicked(bool checked)
{
    L_project = checked;
}



/**
 * @brief onLog
 * 打印日志
 *
 * @param log
 */
void Perseus::onLog(const QString &log)
{
    ui->output_plaintextedit->appendHtml(log);
}



/**
 * @brief onZoomUi
 * 字体缩放实现
 *
 * @param delta
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



/**
 * @brief onToolChainMissing
 * 工具 丢失 请求 安装
 *
 * @param config
 */
void Perseus::onToolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
{
    SUC("open download windows...", "OnToolChainMissing");
    LosView::LosToolMissUi dialog(config, this);
    dialog.exec();
}



/**
 * @brief onDebounceTimeOut
 * onDebounceTimeOut
 */
void Perseus::onDebounceTimeOut()
{
    emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
}



/**
 * @brief onDirectoryChanged
 */
void Perseus::onDirectoryChanged()
{
    L_timer->start();
}



/**
 * @brief OnTogglePanelBtnClicked
 * Ctrl+J 切换 bottom_tabwidget 的显示/隐藏
 */
void Perseus::OnTogglePanelBtnClicked()
{
    auto *widget   = ui->bottom_tabwidget;
    auto *splitter = ui->right_splitter;
    const int idx  = splitter->indexOf(widget);
    if (idx < 0)
        return;

    const auto currentSizes  = splitter->sizes();
    const bool visuallyShown = widget->isVisible() && currentSizes.value(idx) > 0;

    if (visuallyShown)
    {
        L_rightSplitterSizes = currentSizes;
        widget->setVisible(false);
    }
    else
    {
        widget->setVisible(true);
        if (!L_rightSplitterSizes.isEmpty() && L_rightSplitterSizes.size() == currentSizes.size() && L_rightSplitterSizes.value(idx) > 0)
        {
            splitter->setSizes(L_rightSplitterSizes);
            return;
        }
        const int total = splitter->height();
        if (total <= 0)
            return;
        QList<int> newSizes = currentSizes;
        const int bottomH   = qMax(200, total * 7 / 10);
        newSizes[idx]       = bottomH;
        const int others    = newSizes.size() - 1;
        if (others > 0)
        {
            const int each = qMax(100, (total - bottomH) / others);
            for (int i = 0; i < newSizes.size(); ++i)
                if (i != idx)
                    newSizes[i] = each;
        }
        splitter->setSizes(newSizes);
    }
}



/**
 * @brief onFileChooseBtnClicked
 */
void Perseus::onFileChooseBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select a file!"));
    if (filePath.isEmpty() || LOS_tabUi == nullptr)
        return;
    LOS_tabUi->closeAllTabs();
    LosModel::LosFilePath projectFile(filePath);
    if (!projectFile.isExist())
        return;
    LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, projectFile.getAbsolutePath());
    SUC("choose a file And the file project dir: " + projectFile.getAbsolutePath(), "Perseus");
    this->OnFileLoaded(true);
}



/**
 * @brief onDirChooseBtnClick
 */
void Perseus::onDirChooseBtnClick()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open a dir!", "", QFileDialog::ShowDirsOnly));
    if (dir.isEmpty() || LOS_tabUi == nullptr)
        return;
    LOS_tabUi->closeAllTabs();
    LosModel::LosFilePath dirPath(dir);
    LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, dirPath.getAbsoluteFilePath());
    SUC("choose a dir: " + dirPath.getAbsoluteFilePath(), "Perseus");
    this->OnFileLoaded(true);
}



/**
 * @brief onBottomVisibilityChanged
 */
void Perseus::onBottomVisibilityChanged(int index, bool visible)
{
    ui->bottom_tabwidget->setTabVisible(index, visible);
}



/**
 * @brief
 * initConnect
 * - 初始化连接
 * - 工具栏控件由 LosToolBarUi 自己管理, 这里只通过 LosRouter 信号订阅行为
 */
void Perseus::initConnect()
{
    connect(&LosCore::LosLog::instance(), &LosCore::LosLog::_sendLog, this, &Perseus::onLog);
    LOS_tabUi        = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
    LOS_cmdPalette   = new LosView::LosCommandUi(this);
    LOS_cmdArg       = new LosView::LosCommandArgsUi(this);
    LOS_runMgr       = new LosCore::LosRunManager(this);
    LOS_lspMgr       = new LosCore::LosLspManager(this);
    LOS_configMgr    = new LosCore::LosConfigManager(this);
    LOS_toolChainMgr = new LosCore::LosToolChainManager(this);
    LOS_scriptRunner = new LosCore::LosScriptRunner(this);
    L_timer          = new QTimer(this);
    LOS_setting      = new LosView::LosSettingsUi(this);
    {
        LosCore::LosGitManager::instance(); // 这边单独调用一下 初始化一下
    }
    L_timer->setSingleShot(true);
    L_timer->setInterval(300);
    L_filesWatcher = new QFileSystemWatcher(this);
    auto &router = LosCore::LosRouter::instance();
    connect(L_timer, &QTimer::timeout, this, &Perseus::onDebounceTimeOut);
    connect(L_filesWatcher, &QFileSystemWatcher::directoryChanged, this, &Perseus::onDirectoryChanged);
    connect(ui->explorer_treeview, &QTreeView::activated, this, &Perseus::onExplorerFileDoubleClicked);
    connect(&router, &LosCore::LosRouter::_cmd_fileSystemChanged, this, [=, this]() { OnFileLoaded(true, false); }, Qt::QueuedConnection);
    connect(&router, &LosCore::LosRouter::_cmd_toolChainMissing, this, &Perseus::onToolChainMissing);
    connect(&router, &LosCore::LosRouter::_cmd_chooseFileBtnClick, this, &Perseus::onFileChooseBtnClicked);
    connect(&router, &LosCore::LosRouter::_cmd_chooseDirBtnClick, this, &Perseus::onDirChooseBtnClick);
    connect(&router, &LosCore::LosRouter::_cmd_runBtnClick, this, &Perseus::onRunSingleFileBtnClicked);
    connect(&router, &LosCore::LosRouter::_cmd_projectBtnToggled, this, &Perseus::onProjectBtnClicked);
    connect(&router, &LosCore::LosRouter::_cmd_settingBtnClick, this, [this]() { LOS_setting->exec(); });
    connect(&router, &LosCore::LosRouter::_cmd_bottomTabVisibilityChanged, this, &Perseus::onBottomVisibilityChanged);
    connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this,
            [this](const QString &name) { this->setStyleSheet(LosCore::LosThemeManager::instance().buildMainQss(name)); });
    connect(ui->act_explorer_btn, &QPushButton::clicked, this, [this]() { ui->left_panel_stack->setCurrentIndex(0); });
    connect(ui->act_extensions_btn, &QPushButton::clicked, this,
            [this]()
            {
                ui->left_panel_stack->setCurrentIndex(1);
                LosCore::LosNet::instance().requestPlugin();
            });
}



/**
 * @brief initStyle
 * - 初始化样式
 */
void Perseus::initStyle()
{
    this->setWindowIcon(QIcon(":/icons/theme.png"));
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
    QApplication::setFont(defaultFont);
    ui->editor_tabwidget->setTabsClosable(false);
    ui->right_splitter->setSizes({8, 1});
    ui->main_splitter->setStretchFactor(0, 1);
    ui->main_splitter->setStretchFactor(1, 4);
    ui->right_splitter->setStretchFactor(0, 4);
    ui->right_splitter->setStretchFactor(1, 1);
    ui->bottom_tabwidget->setTabText(0, QString::fromUtf8(u8"Output"));
    ui->bottom_tabwidget->setTabText(1, QString::fromUtf8(u8"Issues"));
    ui->bottom_tabwidget->setTabText(2, QString::fromUtf8(u8"Terminal"));
    INF("perseus Engine Initialized ... ", "Perseus");
    this->setStyleSheet(LosCore::LosThemeManager::instance().buildMainQss(LosCore::LosThemeManager::instance().currentTheme()));
}



/**
 * @brief initShotcut
 * 绑定快捷键
 */
void Perseus::initShotcut()
{
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::RUN_SINGLE_FILE, this, [this]() { onRunSingleFileBtnClicked(); }, "run single file");
    LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::FILE_OPEN, this, [this]() { onFilesBtnClicked(); }, "open folder");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::CODE_FORMAT, this, [=]() { emit LosCore::LosRouter::instance()._cmd_codeFormat(); }, "format text");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FILE_SAVE, this,
        [=, this]()
        {
            if (LOS_tabUi)
                LOS_tabUi->saveTab();
            if (LOS_configMgr->isInFiles(LosModel::LosFilePath(LOS_tabUi->getCurFilePath()).getBaseFileName()))
            {
                INF("rebuild..", "Perseus");
                emit LosCore::LosRouter::instance()._cmd_lsp_msg_didChangeWatchedFiles(
                    LOS_tabUi -> getCurFilePath(), LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Changed);
            };
        },
        "run single file");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FONT_ZOOM_IN, this,
        [=, this]()
        {
            INF("larger...", "Perseus");
            this->onZoomUi(2);
        },
        "zoom in");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FONT_ZOOM_OUT, this,
        [=, this]()
        {
            INF("smaller...", "Perseus");
            this->onZoomUi(-2);
        },
        "zoom out");
    LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::COMMANDS, this, [this]() { LOS_cmdPalette->showPalette(); });
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::EDIT_UNDO, this,
        [this]()
        {
            auto *edit = qobject_cast<LosView::LosEditorUi *>(LOS_tabUi ? LOS_tabUi->getCurEditor() : nullptr);
            if (edit && edit->document()->isUndoAvailable())
                edit->undo();
        },
        "undo");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::EDIT_REDO, this,
        [this]()
        {
            auto *edit = qobject_cast<LosView::LosEditorUi *>(LOS_tabUi ? LOS_tabUi->getCurEditor() : nullptr);
            if (edit && edit->document()->isRedoAvailable())
                edit->redo();
        },
        "redo");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::EDIT_REDO_ALT, this,
        [this]()
        {
            auto *edit = qobject_cast<LosView::LosEditorUi *>(LOS_tabUi ? LOS_tabUi->getCurEditor() : nullptr);
            if (edit && edit->document()->isRedoAvailable())
                edit->redo();
        },
        "redo (alt)");
    LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::TAB_CLOSE, this,
                                                [this]()
                                                {
                                                    auto edit = LOS_tabUi->getCurEditor();
                                                    if (!edit)
                                                        return;
                                                    if (!edit->property("isPinned").toBool())
                                                    {
                                                        LOS_tabUi->closeTab(LOS_tabUi->getCurEditIndex());
                                                        return;
                                                    }
                                                });
    LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::TOGGLE_BOTTOM_PANEL, this, [this]() { OnTogglePanelBtnClicked(); });
    LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut ::TOGGLE_PREVIEW, this,
                                                [this]() { emit LosCore::LosRouter::instance()._cmd_togglePreview(LOS_tabUi -> getCurFilePath()); });
}



/**
 * @brief initSession
 */
void Perseus::initSession()
{
    LosCommon::LosSession_Constants::Config conf;
    if (!LosCore::LosSession::instance().loadConfig(&conf))
        return;
    if (!conf.L_themeName.isEmpty())
    {
        LosCore::LosThemeManager::instance().setTheme(conf.L_themeName);
    }
    LosModel::LosFilePath file(conf.L_curProDir);
    bool isSuc = file.isExist();
    LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, file);
    if (!LOS_tabUi || !isSuc)
        return;
    connect(
        &LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_fileTreeDone, this,
        [conf, this]()
        {
            for (const auto &file : conf.L_curFilePaths)
            {
                LOS_tabUi->blockSignals(true);
                LOS_tabUi->openFile(file);
                LOS_tabUi->blockSignals(false);
            }

            if (!conf.L_curActiveFile.isEmpty())
            {
                LOS_tabUi->openFile(conf.L_curActiveFile);
            }
            else if (!conf.L_curFilePaths.isEmpty())
            {
                LOS_tabUi->openFile(conf.L_curFilePaths.first());
            }
            if (conf.L_curProDir.isEmpty() || conf.L_curFilePaths.isEmpty())
                return;
            ui->explorer_treeview->expandToFile(conf.L_curFilePaths.first());
        },
        Qt::SingleShotConnection);
    OnFileLoaded(isSuc);
}



/**
 * @brief collectConfig
 * - 收集当前的 信息
 *
 * @return LosCommon::LosSession_Constants::Config
 */
LosCommon::LosSession_Constants::Config Perseus::collectConfig()
{
    LosCommon::LosSession_Constants::Config conf;
    for (const auto &path : LOS_tabUi->getOpenFiles())
    {
        conf.L_curFilePaths.append(path);
    }
    conf.L_curProDir     = LosCore::LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR).getFilePath();
    conf.L_curActiveFile = LOS_tabUi->getCurFilePath();
    conf.L_themeName     = LosCore::LosThemeManager::instance().currentTheme();
    return conf;
}
