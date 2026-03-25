
#pragma once

#include "view/LosEditorUi/LosEditorUi.h"
#include "core/LosShortcutManager/LosShortcutManager.h"

#include "common/constants/ConstantsStr.h"

#include <QDebug>
#include <QObject>
#include <QWidget>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qsharedpointer.h>
#include <qtablewidget.h>
#include <qtabwidget.h>

namespace LosView {
class LosEditorTabUi : public QWidget {
  Q_OBJECT

public: // construct
  explicit LosEditorTabUi(QTabWidget *tabWidget, QWidget *object = nullptr);
  ~LosEditorTabUi() override;

public: // tool
  void closeTab(int index);
  void saveTab();
  void saveAllTabs();
  void openFile(const QString &file_path);

public: // get
  LosEditorUi *getCurEditor();
  int tabCount() const;
  QString getCurFilePath() const;

private slots: // chs
  void onTabCloseRequested(int index);
  void onEditDirty(bool is_dirty);
  void onCtrlSToSaveCurFile();

signals:
  void _textChangedForLsp(const QString &file_path,
                          const QString &file_content);
  void _completionRequest(const QString &file_path, int line, int col);
  void _openFileForLsp(const QString &file_path, const QString &file_content);
  void _whereDefine(int line, int col,const QString& file_path);

private: // init
  void initConnect();
  void initEditor(LosEditorUi *);

private: // params
  QTabWidget *L_tabWidget;
  QMap<QString, LosView::LosEditorUi *> LOS_pathToUi;
};
} // namespace LosView