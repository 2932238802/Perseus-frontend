
#pragma once

#include "core/LosRouter/LosRouter.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "models/LosFileContext/LosFileContext.h"
#include "view/LosEditorUi/LosEditorUi.h"

#include "common/constants/ConstantsStr.h"

#include <QDebug>
#include <QObject>
#include <QWidget>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qsharedpointer.h>
#include <qtablewidget.h>
#include <qtabwidget.h>

namespace LosView
{
class LosEditorTabUi : public QWidget
{
    Q_OBJECT

  public: // construct
    explicit LosEditorTabUi(QTabWidget *tabWidget, QWidget *object = nullptr);
    ~LosEditorTabUi() override;

  public: // tool
    void closeTab(int index);
    void saveTab();
    void saveAllTabs();
    void openFile(const QString &file_path);
    void formatTab();

  public: // get
    LosEditorUi *getCurEditor();
    int tabCount() const;
    QString getCurFilePath() const;

  private slots: // chs
    void onTabCloseRequested(int index);
    void onEditDirty(bool is_dirty);
    void onDefineResult(const QString &file_path, int line);
    void onDoubleClickedOnIssue(const QString &file_path, int line);

  private: // init
    void initConnect();


  private: // params
    QTabWidget *L_tabWidget;
    QMap<QString, LosView::LosEditorUi *> LOS_pathToUi;
};
} // namespace LosView
