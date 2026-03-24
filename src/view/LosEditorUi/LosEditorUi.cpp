#include "LosEditorUi.h"
#include "common/constants/ConstantsClass.h"
#include "models/LosFileContext/LosFileContext.h"
#include "view/LosCompleterUi/LosCompleterUi.h"
#include <qcompleter.h>
#include <qfontmetrics.h>
#include <qglobal.h>
#include <qtextcursor.h>
#include <qtextdocument.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <qtextobject.h>
#include <qvariant.h>

namespace LosView {

LosEditorUi::LosEditorUi(QWidget *parent) : QPlainTextEdit{parent} {
  initConnect();
  initStyle();
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
展示弹窗
*/
void LosEditorUi::showCompletion(const QStringList &list) {
  if (list.empty() || nullptr == LOS_completer)
    return;

  LOS_completer->updateCompletionList(list);

  QRect r = cursorRect(); // 如何理解 cursorRect
  int idealWidth = LOS_completer->popup()->sizeHintForColumn(0);
  int idealVertical =
      LOS_completer->popup()->verticalScrollBar()->sizeHint().width();
  r.setWidth(idealWidth + idealVertical);
  LOS_completer->setCompletionPrefix(getWordUnderCursor());
  LOS_completer->complete(r);
}

void LosEditorUi::showDiagnostic(const QString &file_path,
                                 const QList<LosCommon::LosDiagnostic> &dias) {
  if (file_path != LOS_filePath->getFilePath())
    return;

  QList<QTextEdit::ExtraSelection> selectionsList; // 如何理解？ ExtraSelection

  for (const auto &a : dias) {
    QTextCharFormat format; // 如何理解？
    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    QTextEdit::ExtraSelection selections;
    switch (a.ds) {
    case LosCommon::DiagnosticSeverity::Information: {
      format.setUnderlineColor(Qt::blue);
      break;
    }
    case LosCommon::DiagnosticSeverity::Error: {
      format.setUnderlineColor(Qt::red);
      break;
    }
    case LosCommon::DiagnosticSeverity::Hint: {
      format.setUnderlineColor(Qt::gray);
      break;
    }
    case LosCommon::DiagnosticSeverity::Warning: {
      format.setUnderlineColor(QColor(255, 165, 0));
      break;
    }
    default: {
      format.setUnderlineColor(Qt::red);
      break;
    }
    }
    format.setToolTip(a.message);
    selections.format = format;

    QTextDocument *doc = this->document();
    QTextBlock startBlock = doc->findBlockByNumber(a.startLine);
    int startPos = startBlock.position() + a.startChar;
    QTextBlock endBlock = doc->findBlockByNumber(a.endLine);
    int endPos = endBlock.position() + a.endChar;

    QTextCursor cursor(doc);
    cursor.setPosition(startPos);
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);

    selections.cursor = cursor;
    selectionsList.append(selections);
  }
  this->setExtraSelections(selectionsList);
}

/**
导入内容
*/
void LosEditorUi::loadContextAndPath(LosModel::LosFileContext *context,
                                     LosModel::LosFilePath *file_path) {
  if (!context || !file_path)
    return;
  LOS_context = context;
  LOS_filePath = file_path;
  blockSignals(true);
  QString text = LOS_context->load(LOS_filePath->getFilePath());
  setPlainText(text);
  blockSignals(false);
  qDebug() << "[Editor] emit open signals:" << LOS_filePath->getFilePath();
  emit _openFileForLsp(LOS_filePath->getFilePath(), this->toPlainText());
}

/**
词汇补全
*/
void LosEditorUi::insertCompletion(const QString &completion) {
  QTextCursor qtc = textCursor();
  int needLenth = completion.size() - LOS_completer->completionPrefix().size();
  qtc.movePosition(QTextCursor::Left);
  qtc.movePosition(QTextCursor::EndOfWord);
  qtc.insertText(completion.right(needLenth));
  setTextCursor(qtc); // 为什么 这里还要 setTextCursor
}

/**
调用 LosContext 接口
*/
bool LosEditorUi::save() {
  if (!LOS_context || !LOS_filePath)
    return false;
  bool ok = LOS_context->save(this->toPlainText(), LOS_filePath->getFilePath());
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
get
获取当前光标下的单词
*/
QString LosEditorUi::getWordUnderCursor() const {
  QTextCursor qtc = textCursor();
  qtc.select(QTextCursor::WordUnderCursor);
  return qtc.selectedText();
}

/**
- 文字稍微 变动 就变脏
*/
void LosEditorUi::initConnect() {
  L_timer = new QTimer(this);
  L_timer->setSingleShot(true);
  LOS_completer = new LosView::LosCompleterUi(this);
  LOS_completer->setWidget(this);

  // activated 有两种
  connect(LOS_completer, QOverload<const QString &>::of(&QCompleter::activated),
          this, &LosEditorUi::insertCompletion);
  connect(this, &QPlainTextEdit::textChanged, this,
          &LosEditorUi::onTextChanged);
  connect(L_timer, &QTimer::timeout, this, [=]() {
    if (LOS_filePath && LOS_context) {
      emit _textChangedForLsp(LOS_filePath->getFilePath(), this->toPlainText());
    }
  });
}

void LosEditorUi::initStyle() {
  QFontMetrics met(this->font());
  int tab = 4 * met.horizontalAdvance(" ");
  this->setTabStopDistance(tab);
}

/**
- 变脏的信号
*/
void LosEditorUi::onTextChanged() {
  if (!LOS_context)
    return;
  if (!LOS_context->isDirty()) {
    LOS_context->setDirty(true);
    emit _editorDirty(true);
  }
  L_timer->start(500);
  QTextCursor cursor = this->textCursor();
  int line = cursor.blockNumber();
  int col = cursor.positionInBlock();
  if (col > 0) {
    QString currentLineText = cursor.block().text();
    QString lastChar = currentLineText.mid(col - 1, 1);
    QString lastTwoChars = col > 1 ? currentLineText.mid(col - 2, 2) : "";

    if (lastChar == "." || lastTwoChars == "->" || lastTwoChars == "::") {
      qDebug() << "[Editor Radar] 探测到补全触发符，坐标 -> 行:" << line
               << " 列:" << col;

      L_timer->stop();
      emit _textChangedForLsp(LOS_filePath->getFilePath(), this->toPlainText());
      emit _completionRequest(LOS_filePath->getFilePath(), line, col);
    }
  }
}

/**
光标拦截
*/
void LosEditorUi::keyPressEvent(QKeyEvent *event) {
  if (LOS_completer && LOS_completer->popup() &&
      LOS_completer->popup()->isVisible()) {
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
      // 悬浮框显示时，把这些按键让给悬浮框去处理
      event->ignore();
      return;
    default:
      break;
    }
  }
  QPlainTextEdit::keyPressEvent(event);
}

} // namespace LosView