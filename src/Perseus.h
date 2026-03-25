#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "core/log/LosLog/LosLog.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosLsp/LosLspManager/LosLspManager.h"
#include "core/LosRunner/LosRunManager/LosRunManager.h"
#include "models/LosFileNode/LosFileNode.h"
#include "models/LosFileTreeModel/LosFileTreeModel.h"
#include "view/LosEditorTabUi/LosEditorTabUi.h"
#include "view/LosEditorUi/LosEditorUi.h"
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
#include <QShortcut>

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
    void updateExplorer();

  public slots:
    void OnFileLoaded(bool isc);
    void onFilesBtnClicked();
    void onExplorerFileDoubleClicked(const QModelIndex &index);
    void onRunSingleFileBtnClicked();
    void onAppendErr(const QString &);
    void onAppendLog(const QString &);
    void onBuildOver(bool);
    void onCompletionTips(const QStringList &);
    void onTextChange_LSP(const QString &filePath, const QString &text);
    void onCompletionRequest_LSP(const QString &filePath, int line, int col);
    void onOpenFile_LSP(const QString &file_path, const QString &file_content);
    void onWhereDefine_LSP(int line, int col, const QString &file_path);
    void onDiagnostics(const QString &file_path, const QList<LosCommon::LosDiagnostic> &);
    void onLog(const QString &log);
    void onDoubleClickedIssuesUi(const QString &file_path, int line);
    void onDefinitionResult(const QString &file_path, int line);

  private:
    Ui::Perseus *ui;
    LosModel::LosFilePath LOS_projectFilepath;
    LosModel::LosFileNode *LOS_rootNode       = nullptr;
    LosModel::LosFileTreeModel *LOS_treeModel = nullptr;
    LosView::LosEditorTabUi *LOS_tabUi        = nullptr;
    LosCore::LosRunManager *LOS_runMgr        = nullptr;
    LosCore::LosLspManager *LOS_lspMgr        = nullptr;
};
