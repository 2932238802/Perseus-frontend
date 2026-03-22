
#include "LosEditorTabUi.h"
#include "models/LosFileContext/LosFileContext.h"
#include "ui/LosEditorUi/LosEditorUi.h"
#include <qdebug.h>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qsharedpointer.h>
#include <qtabwidget.h>
#include <qwidget.h>

namespace LosView {

LosEditorTabUi::LosEditorTabUi(QTabWidget *tab_widget, QWidget *parent)
    : L_tabWidget{tab_widget}, QWidget{parent} {
  initConnect();
}

LosEditorTabUi::~LosEditorTabUi() {}

/**
打开文件
*/
void LosEditorTabUi::openFile(const QString &file_path) {
  if (LOS_pathToUi.contains(file_path)) {
    LosEditorUi *editor = LOS_pathToUi.value(file_path);
    L_tabWidget->setCurrentWidget(editor);
    return;
  }

  LosEditorUi *editor = LosEditorUi::create(this);
  connect(editor, &LosEditorUi::_editorDirty, this,
          &LosEditorTabUi::onEditDirty);
  LosModel::LosFileContext *context = LosModel::LosFileContext::create();
  if (!context->load(file_path)) {
    delete editor;
    delete context;
    return;
  }
  editor->loadContext(context);
  L_tabWidget->addTab(editor, QFileInfo(file_path).fileName());
  LOS_pathToUi.insert(file_path, editor);
  L_tabWidget->setCurrentWidget(editor); // 补充 直接 把界面切过去
}

/**
关闭 ui 点击
*/
void LosEditorTabUi::onTabCloseRequested(int index) {
  QWidget *wi = L_tabWidget->widget(index);
  if (!wi)
    return;
  LosEditorUi *editor = qobject_cast<LosEditorUi *>(wi);


  QString filePath = LOS_pathToUi.key(editor);
  if (!filePath.isEmpty())
    LOS_pathToUi.remove(filePath);
  L_tabWidget->removeTab(index);
  editor->deleteLater();
}

/**
如果编辑器 修改
*/
void LosEditorTabUi::onEditDirty(bool is_dirty) {

  LosView::LosEditorUi *edit = qobject_cast<LosView::LosEditorUi *>(sender());
  if (!edit)
    return;

  int index = L_tabWidget->indexOf(edit);
  if (index == -1)
    return;
  QString currentTitle = L_tabWidget->tabText(index);
  if (is_dirty && !currentTitle.endsWith("*")) {
    L_tabWidget->setTabText(index, currentTitle + " *");
  } else if (!is_dirty && currentTitle.endsWith("*")) {
    L_tabWidget->setTabText(index,
                            currentTitle.left(currentTitle.length() - 2));
  }
}


/**
保存的逻辑
*/
void LosEditorTabUi::onCtrlSToSaveCurFile(){
  auto curWidget = qobject_cast<LosEditorUi*>( L_tabWidget->currentWidget());
  if(!curWidget || !curWidget->isDirty()) return;
  if(!curWidget->save())
  {
    QMessageBox::critical(this,"error","error in save file");
  }
}

/**
初始化
*/
void LosEditorTabUi::initConnect() {
  connect(L_tabWidget, &QTabWidget::tabCloseRequested, this,
          &LosEditorTabUi::onTabCloseRequested);
}
} // namespace LosView