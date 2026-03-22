
#pragma once

#include "ui/LosEditorUi/LosEditorUi.h"
#include <QWidget>
#include <qobject.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <qwidget.h>

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
  int tabCount();

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