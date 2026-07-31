// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosSessionClass.h"
#include <QList>
#include <QMainWindow>

class QFileSystemWatcher;
class QKeyEvent;
class QModelIndex;
class QTimer;

namespace LosCore
{
    class LosConfigManager;
    class LosLspManager;
    class LosRunManager;
    class LosScriptRunner;
    class LosToolChainManager;
} // namespace LosCore

namespace LosModel
{
    class LosFileNode;
    class LosFileTreeModel;
} // namespace LosModel

namespace LosView
{
    class LosAuthUi;
    class LosCommandArgsUi;
    class LosEditorTabUi;
    class LosSettingsUi;
} // namespace LosView

namespace LosCommon
{
    namespace LosSession_Constants
    {
        struct Config;
    }
    namespace LosToolChain_Constants
    {
        struct ToolChainConfig;
    }
} // namespace LosCommon

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
    void OnTogglePanelBtnClicked();
    void onFileChooseBtnClicked();
    void onDirChooseBtnClick();
    void onBottomVisibilityChanged(int, bool);

  private:
    Ui::Perseus *ui;
    bool L_project                                 = false;
    bool L_loggedIn                                = false;
    int L_buildGeneration                          = 0;
    QFileSystemWatcher *L_filesWatcher             = nullptr;
    QTimer *L_timer                                = nullptr;
    QList<int> L_rightSplitterSizes                = {};
    LosView::LosSettingsUi *LOS_setting            = nullptr;
    LosView::LosAuthUi *LOS_auth                   = nullptr;
    LosModel::LosFileNode *LOS_rootNode            = nullptr;
    LosModel::LosFileTreeModel *LOS_treeModel      = nullptr;
    LosView::LosEditorTabUi *LOS_tabUi             = nullptr;
    LosCore::LosRunManager *LOS_runMgr             = nullptr;
    LosCore::LosLspManager *LOS_lspMgr             = nullptr;
    LosCore::LosConfigManager *LOS_configMgr       = nullptr;
    LosCore::LosToolChainManager *LOS_toolChainMgr = nullptr;
    LosView::LosCommandArgsUi *LOS_cmdArg          = nullptr;
    LosCore::LosScriptRunner *LOS_scriptRunner     = nullptr;
};
