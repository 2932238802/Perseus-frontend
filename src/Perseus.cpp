// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "Perseus.h"
#include "./ui_Perseus.h"
#include "common/constants/ConstantsClass/LosSessionClass.h"
#include "common/constants/ConstantsNum/PerseusNum.h"
#include "common/constants/ConstantsStr/LosConfigStr.h"
#include "common/constants/ConstantsStr/LosStateStr.h"
#include "common/constants/ConstantsStr/ShortCut.h"
#include "core/LosAgent/LosAgentManager/LosAgentManager.h"
#include "core/LosConfig/LosConfigManager/LosConfigManager.h"
#include "core/LosFont/LosFontManager.h"
#include "core/LosGitManager/LosGitManager.h"
#include "core/LosLog/LosLog.h"
#include "core/LosLsp/LosLspManager/LosLspManager.h"
#include "core/LosNet/LosNet.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosRunManager/LosRunManager.h"
#include "core/LosRunner/LosScriptRunner/LosScriptRunner.h"
#include "core/LosSesssion/LosSesssion.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/LosState/LosState.h"
#include "core/LosTheme/LosThemeManager.h"
#include "core/LosToolChainManager/LosToolChainManager.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "view/LosAuthUi/LosAuthUi.h"
#include "view/LosCommandArgsUi/LosCommandArgsUi.h"
#include "view/LosDialog/LosDialog.h"
#include "view/LosEditorTabUi/LosEditorTabUi.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include "view/LosSettingsUi/LosSettingsUi.h"
#include "view/LosToolMissUi/LosToolMissUi.h"
#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFileSystemWatcher>
#include <QKeyEvent>
#include <QMessageBox>
#include <QStyle>
#include <QTimer>
#include <qfontmetrics.h>


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
     * 绛夊緟 100 姣鍚庯紝
     * 璋冪敤褰撳墠瀵硅薄鐨?initSession () 鍑芥暟
     * 鍙墽琛屼竴娆℃墽琛屽畬灏辩粨鏉?     */
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
                                             if (dir == buildDirPath || dir.startsWith(buildDirPath + QDir::separator()))
                                                 continue;
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
 * - 鏂囦欢鎸夐挳 琚偣鍑? * - 鏀寔瀵煎叆鏂囦欢鍜屾枃浠跺す
 * - 浠ユ枃浠跺す鎵€鍦ㄧ殑缁濆浣嶇疆 浣滀负 椤圭洰鏍圭洰褰? * - 鍒囨崲鐩綍鏃跺叧闂叾瀹?Tab
 */
void Perseus::onFilesBtnClicked()
{
    QString pathChoose = LosView::LosDialog::pickDir(this, tr("Open Project Folder"));
    if (pathChoose.isEmpty())
    {
        QString filePath = LosView::LosDialog::pickFile(this, tr("Select a file to open its directory"));
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
 * 鍙屽嚮鏂囦欢
 * - 淇 灞曞紑鐨勯棶棰? * - 榛樿宸茬粡鏈?灞曞紑鐨勯棶棰? *
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
 * 鍗?鏂囦欢閫夋嫨
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
 * @brief 椤圭洰 鎸夐挳鐨勭偣鍑? *
 * @param checked
 */
void Perseus::onProjectBtnClicked(bool checked)
{
    L_project = checked;
}



/**
 * @brief onLog
 * 鎵撳嵃鏃ュ織
 * @param log
 */
void Perseus::onLog(const QString &log)
{
    ui->output_plaintextedit->appendHtml(log);
}



/**
 * @brief onZoomUi
 * 瀛椾綋缂╂斁瀹炵幇
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

    const int fontHeight       = QFontMetrics(font).height();
    const int buttonSize       = qMax(48, fontHeight + 20);
    const int activityBarWidth = buttonSize + 4;

    ui->act_explorer_btn->setFixedSize(buttonSize, buttonSize);
    ui->act_extensions_btn->setFixedSize(buttonSize, buttonSize);
    ui->act_auth_btn->setFixedSize(buttonSize, buttonSize);

    ui->activity_bar_widget->setFixedWidth(activityBarWidth);
    const int treeIconSize = qBound(LosCommon::Perseus_Constants::TREE_ICON_MIN, fontHeight, LosCommon::Perseus_Constants::TREE_ICON_MAX);
    ui->explorer_treeview->setIconSize(QSize(treeIconSize, treeIconSize));
    auto refreshWidget = [](QWidget *widget)
    {
        if (widget == nullptr)
        {
            return;
        }
        widget->updateGeometry();
        if (QLayout *layout = widget->layout())
        {
            layout->invalidate();
            layout->activate();
        }
        widget->update();
    };
    refreshWidget(this);
    refreshWidget(centralWidget());
    refreshWidget(ui->toolbar_widget);
    refreshWidget(ui->activity_bar_widget);
    refreshWidget(ui->left_panel_stack);
    refreshWidget(ui->explorer_treeview);
    refreshWidget(ui->right_splitter);
    refreshWidget(ui->agent_panel);
    refreshWidget(ui->statusbar_widget);
    ui->explorer_treeview->doItemsLayout();
    QApplication::processEvents();
}



/**
 * @brief onToolChainMissing
 * 宸ュ叿 涓㈠け 璇锋眰 瀹夎
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
 * Ctrl+J 鍒囨崲 bottom_tabwidget 鐨勬樉绀?闅愯棌
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
    QString filePath = LosView::LosDialog::pickFile(this, tr("Select a file!"));
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
    QString dir = LosView::LosDialog::pickDir(this, tr("Open a dir!"));
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
 * - 鍒濆鍖栬繛鎺? * - 宸ュ叿鏍忔帶浠剁敱 LosToolBarUi 鑷繁绠＄悊, 杩欓噷鍙€氳繃 LosRouter
 * 淇″彿璁㈤槄琛屼负
 */
void Perseus::initConnect()
{
    connect(&LosCore::LosLog::instance(), &LosCore::LosLog::_sendLog, this, &Perseus::onLog);
    LOS_tabUi        = new LosView::LosEditorTabUi(ui->editor_tabwidget, this);
    LOS_cmdArg       = new LosView::LosCommandArgsUi(this);
    LOS_runMgr       = new LosCore::LosRunManager(this);
    LOS_lspMgr       = new LosCore::LosLspManager(this);
    LOS_configMgr    = new LosCore::LosConfigManager(this);
    LOS_toolChainMgr = new LosCore::LosToolChainManager(this);
    LOS_scriptRunner = new LosCore::LosScriptRunner(this);
    L_timer          = new QTimer(this);
    LOS_setting      = new LosView::LosSettingsUi(this);
    LOS_auth         = new LosView::LosAuthUi(this);
    {
        LosCore::LosGitManager::instance(); // 杩欒竟鍗曠嫭璋冪敤涓€涓?鍒濆鍖栦竴涓?        LosCore::LosNet::instance();          //
                                            // 鍒濆鍖栫綉缁滃眰 (娉ㄥ唽/鐧诲綍淇″彿鐩戝惉)
        LosCore::LosAgentManager::instance(); // 鍒濆鍖?Agent 涓氬姟灞?(鐩戝惉 _cmd_agent_sendMessage)
    }
    L_timer->setSingleShot(true);
    L_timer->setInterval(300);
    L_filesWatcher = new QFileSystemWatcher(this);
    auto &router   = LosCore::LosRouter::instance();
    auto &net      = LosCore::LosNet::instance();
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
    connect(&router, &LosCore::LosRouter::_cmd_fontChanged, this, [this](const QString &)
            { this->setStyleSheet(LosCore::LosThemeManager::instance().buildMainQss(LosCore::LosThemeManager::instance().currentTheme())); });
    connect(ui->act_explorer_btn, &QPushButton::clicked, this, [this]() { ui->left_panel_stack->setCurrentIndex(0); });
    connect(ui->act_extensions_btn, &QPushButton::clicked, this,
            [this]()
            {
                ui->left_panel_stack->setCurrentIndex(1);
                LosCore::LosNet::instance().requestPlugin();
            });
    connect(ui->act_auth_btn, &QPushButton::clicked, this, []() { emit LosCore::LosRouter::instance()._cmd_authBtnClick(); });
    connect(&router, &LosCore::LosRouter::_cmd_authBtnClick, this,
            [this]()
            {
                if (!L_loggedIn)
                {
                    LOS_auth->exec();
                    return;
                }
                auto ret = QMessageBox::question(this, QStringLiteral("璐︽埛"), QStringLiteral("纭畾瑕佺櫥鍑哄悧?"),
                                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if (ret == QMessageBox::Yes)
                {
                    emit LosCore::LosRouter::instance()._cmd_auth_loginStateChanged(false);
                }
            });
    connect(&router, &LosCore::LosRouter::_cmd_auth_loginStateChanged, this,
            [this](bool loggedIn)
            {
                L_loggedIn = loggedIn;
                ui->act_auth_btn->setProperty("logged", loggedIn);
                ui->act_auth_btn->style()->unpolish(ui->act_auth_btn);
                ui->act_auth_btn->style()->polish(ui->act_auth_btn);
            });
    connect(&router, &LosCore::LosRouter::_cmd_needAuth, this, [this]() { LOS_auth->exec(); });
}



/**
 * @brief initStyle
 * - 鍒濆鍖栨牱寮? */
void Perseus::initStyle()
{
    this->setWindowIcon(QIcon(":/icons/theme.png"));
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(12);
    QApplication::setFont(defaultFont);
    ui->editor_tabwidget->setTabsClosable(false);
    ui->right_splitter->setSizes({8, 1});
    ui->main_splitter->setStretchFactor(0, 1); // left_panel_stack
    ui->main_splitter->setStretchFactor(1, 4); // right_splitter (缂栬緫鍣?搴曢儴)
    ui->main_splitter->setStretchFactor(2, 1); // agent_panel (鍙充晶 Agent 渚ц竟鏍?
    ui->main_splitter->setSizes({200, 760, 280});
    ui->main_splitter->setChildrenCollapsible(false);
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
 * 缁戝畾蹇嵎閿? */
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
            this->onZoomUi(LosCommon::Perseus_Constants::ZOOM_DELTA);
        },
        "zoom in");
    LosCore::LosShortcutManager::instance().reg(
        LosCommon::ShortCut::FONT_ZOOM_OUT, this,
        [=, this]()
        {
            INF("smaller...", "Perseus");
            this->onZoomUi(-1 * LosCommon::Perseus_Constants::ZOOM_DELTA);
        },
        "zoom out");
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
    if (!conf.L_fontName.isEmpty())
    {
        LosCore::LosFontManager::instance().setFontFamily(conf.L_fontName);
    }
    LosModel::LosFilePath file(conf.L_curProDir);
    bool isSuc = file.isExist();
    LosCore::LosState::instance().set<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR, file);
    LosCore::LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN, conf.LOS_authConfig.L_token);
    LosCore::LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_USERNAME, conf.LOS_authConfig.L_username);
    LosCore::LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::CLANG_FORMAT, conf.LOS_formatConfig.L_clangFormat);
    // 璁剧疆瀛椾綋
    // 瀹氫箟
    const int fontSize = qBound(LosCommon::Perseus_Constants::ZOOM_MIN, conf.LOS_formatConfig.L_fontSize, LosCommon::Perseus_Constants::ZOOM_MAX);
    QFont font         = QApplication::font();
    font.setPointSize(fontSize);
    QApplication::setFont(font);
    onZoomUi(0);

    if (!conf.LOS_authConfig.L_token.isEmpty())
    {
        emit LosCore::LosRouter::instance()._cmd_auth_autoLogin_request(conf.LOS_authConfig.L_token);
    }
    if (!LOS_tabUi || !isSuc)
        return;

    // 鏂囦欢鏍?鍒涘缓
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
 * - 鏀堕泦褰撳墠鐨?淇℃伅
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
    conf.L_fontName      = LosCore::LosFontManager::instance().currentFontFamily();
    LosCommon::LosSession_Constants::AuthConfig authConfig;
    authConfig.L_username = LosCore::LosState::instance().get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_USERNAME);
    authConfig.L_token    = LosCore::LosState::instance().get<QString>(LosCommon::LosState_Constants::SG_STR::AUTH_TOKEN);
    LosCommon::LosSession_Constants::FormatConfig formatConfig;
    formatConfig.L_fontSize    = QApplication::font().pointSize();
    formatConfig.L_clangFormat = LosCore::LosState::instance().get<QString>(LosCommon::LosState_Constants::SG_STR::CLANG_FORMAT);
    conf.LOS_formatConfig      = formatConfig;
    conf.LOS_authConfig        = authConfig;
    return conf;
}
