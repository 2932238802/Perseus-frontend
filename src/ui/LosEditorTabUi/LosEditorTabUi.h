
#pragma once

#include "ui/LosEditorUi/LosEditorUi.h"
#include <QWidget>
#include <QDebug>
#include <qglobal.h>
#include <qmessagebox.h>
#include <QObject>
#include <qsharedpointer.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <QWidget>

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

private: // init
  void initConnect();

private: // params
  QTabWidget *L_tabWidget;
  QMap<QString, LosView::LosEditorUi *> LOS_pathToUi;
};
} // namespace LosView