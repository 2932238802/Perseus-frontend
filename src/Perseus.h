#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosConfig/LosConfigManager/LosConfigManager.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosLsp/LosLspManager/LosLspManager.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosRunManager/LosRunManager.h"
#include "core/LosSesssion/LosSesssion.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/LosState/LosState.h"
#include "core/LosToolChainManager/LosToolChainManager.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFilePath/LosFilePath.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "view/LosEditorTabUi/LosEditorTabUi.h"
#include "view/LosEditorUi/LosEditorUi.h"
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
#include <qpushbutton.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Perseus;
}
QT_END_NAMESPACE

class Perseus : public QMainWindow
{
    Q_OBJECT
  public:
    Perseus(QWidget *parent = nullptr);
    ~Perseus();

  protected:
    void keyPressEvent(QKeyEvent *) override;

  private:
    void initConnect();
    void initStyle();
    void initShotcut();
    void initSession();

  private: // tool
    LosCommon::LosSession_Constants::Config collectConfig();

  public slots:
    void OnFileLoaded(bool isc);
    void onFilesBtnClicked();                                   // 打开本地文件夹
    void onExplorerFileDoubleClicked(const QModelIndex &index); // 打开本地文件夹
    void onRunSingleFileBtnClicked();                           // 运行一个文本
    void onProjectBtnClicked(bool checked);                     // 是否开启 项目 模式
    void onLog(const QString &log);
    void onZoomUi(int delta);
    void onToolChainMissing(const LosCommon::LosToolChain_Constants::ToolChainConfig &);

  private:
    Ui::Perseus *ui;
    bool L_project                                 = false;
    LosModel::LosFileNode *LOS_rootNode            = nullptr;
    LosModel::LosFileTreeModel *LOS_treeModel      = nullptr;
    LosView::LosEditorTabUi *LOS_tabUi             = nullptr;
    LosCore::LosRunManager *LOS_runMgr             = nullptr;
    LosCore::LosLspManager *LOS_lspMgr             = nullptr;
    LosCore::LosConfigManager *LOS_configMgr       = nullptr;
    LosCore::LosToolChainManager *LOS_toolChainMgr = nullptr;
};
