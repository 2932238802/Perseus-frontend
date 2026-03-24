#include "LosEditorUi.h"
#include "models/LosFileContext/LosFileContext.h"
#include <qdebug.h>
#include <qmessagebox.h>

namespace LosView {

LosEditorUi::LosEditorUi(QWidget *parent) : QPlainTextEdit{parent} {
  initConnect();
}
LosEditorUi::~LosEditorUi() {
  if (LOS_context) {
    delete LOS_context;
    LOS_context = nullptr;
  }
}

LosEditorUi *LosEditorUi::create(QWidget *parent) {
  LosEditorUi *editor = new LosEditorUi(parent); // 补充
  return editor;
}

/**
导入内容
*/
void LosEditorUi::loadContext(LosModel::LosFileContext *context) {
  if (!context)
    return;
  blockSignals(true);
  setPlainText(context->getContent());
  blockSignals(false);
  LOS_context = context;
}

/**
调用 LosContext 接口
*/
bool LosEditorUi::save() {
  LOS_context->setContent(this->toPlainText());
  bool ok = LOS_context->save();
  if (ok) {
    LOS_context->setDirty(false);
    emit _editorDirty(false);
  }
  return ok;
}

/**
是不是脏的
*/
bool LosEditorUi::isDirty() const { return LOS_context->isDirty(); }

/**
- 文字稍微 变动 就变脏
*/
void LosEditorUi::initConnect() {
  connect(this, &QPlainTextEdit::textChanged, this,
          &LosEditorUi::onTextChanged);
}

/**
- 变脏的信号
*/
void LosEditorUi::onTextChanged() {
  if (!LOS_context->isDirty()) {
    LOS_context->setDirty(true);
    emit _editorDirty(true);
  }
}

} // namespace LosView