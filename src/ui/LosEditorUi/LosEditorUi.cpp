#include "LosEditorUi.h"
#include "models/LosFileContext/LosFileContext.h"
#include <qmessagebox.h>

namespace LosView {

LosEditorUi::LosEditorUi(QWidget *parent) : QPlainTextEdit{parent} {
  initConnect();
}
LosEditorUi::~LosEditorUi() {}

LosEditorUi *LosEditorUi::create(QWidget *parent) {
  LosEditorUi *editor = new LosEditorUi(parent); // 补充
  return editor;
}

void LosEditorUi::loadContext(LosModel::LosFileContext *context) {
  if (!context)
    return;
  blockSignals(true);
  setPlainText(context->getContent());
  blockSignals(false);
  LOS_context = context;
}
bool LosEditorUi::save() { return LOS_context->save(); }

bool LosEditorUi::isDirty() const { return LOS_context->isDirty(); }

void LosEditorUi::initConnect() {
  connect(this, &QPlainTextEdit::textChanged, this,
          &LosEditorUi::onTextChanged);
}

void LosEditorUi::onTextChanged() {
  if (!L_isDirty) {
    L_isDirty = true;
    emit _editorDirty(true);
  }
}

} // namespace LosView