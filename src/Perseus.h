#pragma once

#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr/ShortCut.h"
#include "core/LosConfig/LosConfigManager/LosConfigManager.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosLsp/LosLspManager/LosLspManager.h"
#include "core/LosNet/LosNet.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosRunManager/LosRunManager.h"
#include "core/LosRunner/LosScriptRunner/LosScriptRunner.h"
#include "core/LosSesssion/LosSesssion.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/LosState/LosState.h"
#include "core/LosToolChainManager/LosToolChainManager.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "view/LosCommandArgsUi/LosCommandArgsUi.h"
#include "view/LosCommandUi/LosCommandUi.h"
#include "view/LosEditorTabUi/LosEditorTabUi.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include "view/LosFloatingPanelUi/LosFloatingPanelUi.h"
#include "view/LosFloatingPanelUi/LosGotoLinePopupUi/LosGotoLinePopupUi.h"
#include "view/LosSettingsUi/LosSettingsUi.h"
#include "view/LosToolMissUi/LosToolMissUi.h"
#include "view/style/perseus_style.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QKeySequence>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QPointer>
#include <QShortcut>
#include <QTimer>
#include <qfiledialog.h>
#include <qfilesystemwatcher.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qtimer.h>
#include <qtmetamacros.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Perseus;
}
QT_END_NAMESPACE

/*
 * Perseus
 * - 根本类
 *
 * keyPressEvent
 * - TODO: 加入 关闭应用程序的快捷键
 *
 * initConnect
 * - 初始化 连接
 * - 初始化 各种类
 *
 * initStyle
 * - 初始化 样式
 *
 * initShotcut
 * - 初始化 快捷键
 *
 * initSession
 * - 初始化 历史会话
 *
 * collectConfig
 * - 收集 当前的文件信息
 * - 保存到本地
 * - 形成记忆
 *
 * OnFileLoaded
 * - 文件加载状态
 * - 完毕
 *   - 建立左侧的文件树
 *   - 更新左侧的文件结构 并打开历史节点
 */
class Perseus : public QMainWindow
{
    Q_OBJECT
  public:
    explicit Perseus(QWidget *parent = nullptr);
    ~Perseus();

  protected: // override
    void keyPressEvent(QKeyEvent *) override;

  private:
    void initConnect();
    void initStyle();
    void initShotcut();
    void initSession();

  private: // tool
    LosCommon::LosSession_Constants::Config collectConfig();

  public slots: // xhc
    void OnFileLoaded(bool isc, bool runAnalysis = true);
    void onFilesBtnClicked();
    void onExplorerFileDoubleClicked(const QModelIndex &index);
    void onRunSingleFileBtnClicked();
    void onProjectBtnClicked(bool checked);
    void onLog(const QString &log);
    void onZoomUi(int delta);
    void onToolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &);
    void onDebounceTimeOut();
    void onDirectoryChanged();

  private:
    Ui::Perseus *ui;
    bool L_project                                 = false;
    int L_buildGeneration                          = 0;
    QFileSystemWatcher *L_filesWatcher             = nullptr;
    QTimer *L_timer                                = nullptr;
    LosModel::LosFileNode *LOS_rootNode            = nullptr;
    LosModel::LosFileTreeModel *LOS_treeModel      = nullptr;
    LosView::LosEditorTabUi *LOS_tabUi             = nullptr;
    LosCore::LosRunManager *LOS_runMgr             = nullptr;
    LosCore::LosLspManager *LOS_lspMgr             = nullptr;
    LosCore::LosConfigManager *LOS_configMgr       = nullptr;
    LosCore::LosToolChainManager *LOS_toolChainMgr = nullptr;
    LosView::LosCommandUi *LOS_cmdPalette          = nullptr;
    LosView::LosCommandArgsUi *LOS_cmdArg          = nullptr;
    LosCore::LosScriptRunner *LOS_scriptRunner     = nullptr;
};
