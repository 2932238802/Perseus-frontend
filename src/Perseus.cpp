#include "Perseus.h"
#include "./ui_Perseus.h"

/*
 * 构造
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



/*
 * 析构
 */
Perseus::~Perseus()
{
    LosCore::LosSession::instance().saveConfig(collectConfig());
    LosCore::LosLog::instance().disconnect(this);
    if (LOS_treeModel)
        delete LOS_treeModel;
    delete ui;
}



/*
 * esc 关闭程序
 */
void Perseus::keyPressEvent(QKeyEvent *e)
{
    QMainWindow::keyPressEvent(e);
}



/*
 * 文件 加载完毕
 */
/*
 * 文件 加载完毕
 */
void Perseus::OnFileLoaded(bool isc, bool run_analysis)
{
    if (!isc)
    {
        ERR("load file failed!", "Perseus");
        return;
    }

    LosModel::LosFilePath projectPath =
        LosCore::LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);

    ui->project_dir_label->setText(projectPath.getFilePath());

    if (!projectPath.isExist())
    {
        ERR("project path does not exist in global state!", "Perseus");
        return;
    }

    QString curPath{projectPath.getFilePath()};
    int L_curGen      = ++L_buildGeneration; /* 记录本次构建的代次 */
    auto *newRootNode = LosModel::LosFileNode::create(curPath, nullptr);
    LosModel::LosFileNode::build(
        newRootNode, curPath,
        [this, curPath, newRootNode, L_curGen, run_analysis]()
        {
            /*
             * 代次守卫
             * - 若当前代次已不是最新（说明在本次 build 完成前OnFileLoaded 又被调用了）
             * - 则丢弃本次结果，避免竞争
             */
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

            /*
             * 只在用户主动打开项目时运行 cmake 分析
             * 文件系统变化（_cmd_fileSystemChanged）仅刷新文件树，不重跑 cmake
             * 避免 cmake 写入 build/ → 触发 watcher → 再次 analyse → 死循环
             */
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

                /*
                 * 递归监听所有子目录
                 * - 避免 cmake 写 build/ 时触发 directoryChanged  _cmd_fileSystemChanged 死循环
                 * - 但排除构建输出目录及版本控制目录
                 */
                const QString buildDirPath = QDir(curPath).filePath(LosCommon::LosConfig_Constants::BUILD_NAME);
                QDirIterator it(curPath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden,
                                QDirIterator::Subdirectories);
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
            if (oldModel)
            {
                oldModel->deleteLater();
            }
        });
}



/*
 * onFilesBtnClicked
 * - 文件按钮 被点击
 * - 支持导入文件和文件夹
 * - 以文件夹所在的绝对位置 作为 项目根目录
 * - 切换目录时关闭其它 Tab
 */
void Perseus::onFilesBtnClicked()
{
    QString pathChoose = QFileDialog::getExistingDirectory(
        this, tr("Open Project Folder"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
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
        LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR,
                                                                 projectFilepath);
        SUC("load " + pathChoose, "Perseus");
        this->OnFileLoaded(isSuc);
    }
}



/*
 * 双击文件
 * - 修复 展开的问题
 * - 默认已经有 展开的问题
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



/*
 * 点击运行按钮
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



/*
 * - 项目 按钮的点击
 */
void Perseus::onProjectBtnClicked(bool checked)
{
    L_project = checked;
}



/*
 * - 打印日志
 */
void Perseus::onLog(const QString &log)
{
    ui->output_plaintextedit->appendHtml(log);
}



/*
 * 字体缩放实现
 * - this->setStyleSheet(this->styleSheet());
 * - 增加对应的 格式 修复
 * -
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

    /*
     * 缩放 之后 重新 适应一下 样式
     */
    this->setStyleSheet(this->styleSheet());
}



/*
 * onToolChainMissing
 * - 工具 丢失 请求 安装
 */
void Perseus::onToolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
{
    SUC("open download windows...", "OnToolChainMissing");
    LosView::LosToolMissUi dialog(config, this);
    dialog.exec();
}



/*
 * onDebounceTimeOut
 * - 时间到了 发送信号
 */
void Perseus::onDebounceTimeOut()
{
    emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
}



/*
 * 文件树 发生 变动的时候
 * 发送一个信号 过去
 */
void Perseus::onDirectoryChanged()
{
    L_timer->start();
}



/**
 * @brief
 * initConnect
 * - 初始化连接
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
    L_timer->setSingleShot(true);
    L_timer->setInterval(300);
    L_filesWatcher = new QFileSystemWatcher(this);
    connect(L_timer, &QTimer::timeout, this, &Perseus::onDebounceTimeOut);
    connect(L_filesWatcher, &QFileSystemWatcher::directoryChanged, this, &Perseus::onDirectoryChanged);
    connect(ui->files_btn, &QPushButton::clicked, this, &Perseus::onFilesBtnClicked);
    connect(ui->explorer_treeview, &QTreeView::activated, this, &Perseus::onExplorerFileDoubleClicked);
    connect(ui->run_singleFile_btn, &QPushButton::clicked, this, &Perseus::onRunSingleFileBtnClicked);
    connect(
        &LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_fileSystemChanged, this,
        [=, this]()
        {
            /*
             * 文件系统变化仅刷新文件树，不重新运行 cmake analyse
             * 防止 cmake 写 build/ → watcher 触发 → 再次 analyse → 死循环
             */
            OnFileLoaded(true, false);
        },
        Qt::QueuedConnection);
    connect(ui->project_btn, &QRadioButton::toggled, this, &Perseus::onProjectBtnClicked);
    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_toolChainMissing, this,
            &Perseus::onToolChainMissing);
    connect(ui->setting_btn, &QPushButton::clicked, this,
            [=, this]()
            {
                LosView::LosSettingsUi settingDialog(this);
                settingDialog.exec();
            });
    connect(ui->act_explorer_btn, &QPushButton::clicked, this, [this]() { ui->left_panel_stack->setCurrentIndex(0); });
    connect(ui->act_extensions_btn, &QPushButton::clicked, this,
            [this]()
            {
                ui->left_panel_stack->setCurrentIndex(1);
                LosCore::LosNet::instance().requestPlugin();
            });

    ui->files_btn->addOption("choose a file",
                             [this]()
                             {
                                 QString filePath = QFileDialog::getOpenFileName(this, tr("Select a file!"));
                                 if (filePath.isEmpty() || LOS_tabUi == nullptr)
                                     return;
                                 LOS_tabUi->closeAllTabs();
                                 LosModel::LosFilePath projectFile(filePath);
                                 if (!projectFile.isExist())
                                     return;
                                 LosCore::LosState::instance().set<LosModel::LosFilePath>(
                                     LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, projectFile.getAbsolutePath());
                                 SUC("choose a file And the file project dir: " + projectFile.getAbsolutePath(),
                                     "Perseus");
                                 this->OnFileLoaded(true);
                             });
    ui->files_btn->addOption(
        "choose a dir",
        [this]()
        {
            QString dir = QFileDialog::getExistingDirectory(this, tr("Open a dir!", "", QFileDialog::ShowDirsOnly));
            if (dir.isEmpty() || LOS_tabUi == nullptr)
                return;
            LOS_tabUi->closeAllTabs();
            LosModel::LosFilePath dirPath(dir);
            LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR,
                                                                     dirPath.getAbsoluteFilePath());
            SUC("choose a dir: " + dirPath.getAbsoluteFilePath(), "Perseus");
            this->OnFileLoaded(true);
        });
    ui->files_btn->addSeparator();
    ui->files_btn->addOption("version",
                             [this]()
                             {
                                 QString versionInfo = QString("<h3>Perseus IDE</h3>"
                                                               "<p><b>Version:</b> 1.0.56 (Build: %1)</p>"
                                                               "<p><b>Qt version:</b> Qt %2</p>"
                                                               "<hr>"
                                                               "<p>Copyright &copy; 2026 LosAngelous</p>")
                                                           .arg(__DATE__)
                                                           .arg(QT_VERSION_STR);
                                 QMessageBox::about(this, tr("About Perseus"), versionInfo);
                             });
}



/*
 * 初始化样式
 */
void Perseus::initStyle()
{
    this->setWindowIcon(QIcon(":/icons/theme.png"));
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
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



/*
 * 快捷键
 */
void Perseus::initShotcut()
{
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::RUN_SINGLE_FILE, this, [this]() { onRunSingleFileBtnClicked(); }, "run single file");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FILE_OPEN, this, [this]() { onFilesBtnClicked(); }, "open folder");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::CODE_FORMAT, this, [=]() { emit LosCore::LosRouter::instance()._cmd_codeFormat(); },
        "format text");
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
                    LOS_tabUi -> getCurFilePath(),
                    LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Changed);
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
    LosCore::LosShortcutManager::instance().reg(LosCommon::ShortCut::COMMANDS, this,
                                                [this]() { LOS_cmdPalette->showPalette(); });
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::GOTO_LINE, this,
        [this]()
        {
            auto editor = LOS_tabUi->getCurEditor();
            if (!editor)
                return;
            int maxLines                               = editor->document()->blockCount();
            LosView::LosGotoLinePopupUi *contentWidget = new LosView::LosGotoLinePopupUi();
            LosView::LosFloatingPanelUi *dialog        = new LosView::LosFloatingPanelUi(contentWidget, true, this);
            connect(contentWidget->getLineEdit(), &QLineEdit::returnPressed, dialog, &QDialog::accept);
            dialog->showAtPosition(editor, LosCommon::LosFloatingPanelUi_Constants::PositionMode::TopRight);
            contentWidget->getLineEdit()->setFocus();
            if (dialog->exec() == QDialog::Accepted)
            {
                int line = contentWidget->getLineNumber();
                if (line > 0)
                {
                    if (line > maxLines)
                    {
                        line = maxLines;
                    }

                    /*
                     * 底层行号减 1
                     */
                    editor->gotoLine(line - 1);
                }
            }
            dialog->deleteLater();
        },
        "go to line");

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
}



/*
 * 初始化会话
 */
void Perseus::initSession()
{
    /*
     * 加载 这个 conf
     */
    LosCommon::LosSession_Constants::Config conf;
    if (!LosCore::LosSession::instance().loadConfig(&conf))
        return;

    /*
     * 打开 file
     * 存到 全局
     */
    LosModel::LosFilePath file(conf.L_curProDir);
    bool isSuc = file.isExist();
    LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, file);

    /*
     * 更新全局 状态
     */
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

            /*
             * 恢复上次激活的标签页
             * 优先使用保存的 L_curActiveFile
             * 若为空则回退到打开列表的第一个文件
             */
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



/*
 * 收集当前的配置
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
    conf.L_curActiveFile = LOS_tabUi->getCurFilePath();
    return conf;
}
