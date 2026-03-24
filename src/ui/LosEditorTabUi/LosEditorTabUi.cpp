
#include "LosEditorTabUi.h"
#include "models/LosFileContext/LosFileContext.h"
#include "ui/LosEditorUi/LosEditorUi.h"
#include <qglobal.h>
#include <qmessagebox.h>
#include <qobject.h>

namespace LosView {

LosEditorTabUi::LosEditorTabUi(QTabWidget *tab_widget, QWidget *parent)
    : L_tabWidget{tab_widget}, QWidget{parent} {
  initConnect();
}

LosEditorTabUi::~LosEditorTabUi() {}

void LosEditorTabUi::closeTab(int index) {}
void LosEditorTabUi::saveTab() {
  if (nullptr == L_tabWidget) {
    return;
  }
  auto widget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
  if (!widget || !widget->isDirty()) {
    return;
  }
  if (!widget->save()) {
    QMessageBox::critical(this, "error", "failed to save file");
  }
}

void LosEditorTabUi::saveAllTabs() {
  if (nullptr == L_tabWidget) {
    QMessageBox::critical(this, "error", "error in saveTab: nullptr");
    return;
  }
  for (int i = 0; i < L_tabWidget->count(); i++) {
    auto a = qobject_cast<LosEditorUi *>(L_tabWidget->widget(i));
    if (!a || !a->isDirty()) {
      continue;
    }
    if (!a->save()) {
      QMessageBox::warning(this, "warning",
                           "error in saveAllTabs: save file [" +
                               LOS_pathToUi.key(a) + "] failed!");
    }
  }
}

/**
tool
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
get
获取当前编辑器
*/
LosEditorUi *LosEditorTabUi::getCurEditor() {
  if (L_tabWidget != nullptr) {
    return qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
  }
  return nullptr;
}

/**
get
*/
int LosEditorTabUi::tabCount() const { return L_tabWidget->count(); }

/**
get
*/
QString LosEditorTabUi::getCurFilePath() const {
  auto widget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
  return widget != nullptr ? LOS_pathToUi.key(widget) : "";
}

/**
关闭 ui 点击
*/
void LosEditorTabUi::onTabCloseRequested(int index) {
  QWidget *wi = L_tabWidget->widget(index);
  if (!wi)
    return;
  LosEditorUi *editor = qobject_cast<LosEditorUi *>(wi);

  // 补充
  if (editor->isDirty()) {    
    QString fileName = L_tabWidget->tabText(index);
    fileName.replace(" *", ""); 
    QMessageBox::StandardButton res = QMessageBox::warning(
        this, "save tips",
        QString("file '%1' has been modified. save changes?").arg(fileName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);
    if (res == QMessageBox::Save) {
        if (!editor->save()) {
            QMessageBox::critical(this, "error", "save failed, unable to close the tab!");
            return; 
        }
    } else if (res == QMessageBox::Cancel) {
        return; 
    }
  }
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
void LosEditorTabUi::onCtrlSToSaveCurFile() {
  auto curWidget = qobject_cast<LosEditorUi *>(L_tabWidget->currentWidget());
  if (!curWidget || !curWidget->isDirty())
    return;
  if (!curWidget->save()) {
    QMessageBox::critical(this, "error", "error in save file");
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