#include "LosEditorUi.h"
#include "core/LosRouter/LosRouter.h"


namespace LosView
{

LosEditorUi::LosEditorUi(QWidget *parent) : QPlainTextEdit{parent}
{
    initConnect();
    initStyle();
}
LosEditorUi::~LosEditorUi()
{
    if (LOS_context)
    {
        delete LOS_context;
        LOS_context = nullptr;
    }
}



/**
展示弹窗
*/
void LosEditorUi::showCompletion(const QStringList &list)
{
    if (list.empty() || nullptr == LOS_completer)
        return;
    QString prefix = getWordUnderCursor();
    if (prefix != LOS_completer->completionPrefix())
    {
        LOS_completer->setCompletionPrefix(prefix);
    }
    LOS_completer->updateCompletionList(list);
    QRect r = cursorRect();
    QFontMetrics fm(this->font());
    int prefixPixelWidth = fm.horizontalAdvance(prefix);
    r.translate(-prefixPixelWidth, 0);                             // 移动到当前 单词的开头
    int idealWidth = LOS_completer->popup()->sizeHintForColumn(0); // 设置
    int padding    = 25;
    int finalWidth = qMin(idealWidth + padding, 500);
    r.setWidth(finalWidth);
    LOS_completer->complete(r);
}



/**
展示错误
*/
void LosEditorUi::showDiagnostic(const QString &file_path,
                                 const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &dias)
{
    if (getWordUnderCursor() != L_oldWord)
    {
        return;
    }

    if (file_path != LOS_filePath->getFilePath())
        return;
    QList<QTextEdit::ExtraSelection> selectionsList;

    for (const auto &a : dias)
    {
        QTextCharFormat format;
        // 波浪线
        format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
        QTextEdit::ExtraSelection selections;
        switch (a.ds)
        {
        case LosCommon::LosLsp_Constants::DiagnosticSeverity::Information:
        {
            format.setUnderlineColor(Qt::blue);
            break;
        }
        case LosCommon::LosLsp_Constants::DiagnosticSeverity::Error:
        {
            format.setUnderlineColor(Qt::red);
            break;
        }
        case LosCommon::LosLsp_Constants::DiagnosticSeverity::Hint:
        {
            format.setUnderlineColor(Qt::gray);
            break;
        }
        case LosCommon::LosLsp_Constants::DiagnosticSeverity::Warning:
        {
            format.setUnderlineColor(QColor(255, 165, 0));
            break;
        }
        default:
        {
            format.setUnderlineColor(Qt::red);
            break;
        }
        }
        format.setToolTip(a.message);
        selections.format = format;

        QTextDocument *doc    = this->document();
        QTextBlock startBlock = doc->findBlockByNumber(a.startLine);
        int startPos          = startBlock.position() + a.startChar;
        QTextBlock endBlock   = doc->findBlockByNumber(a.endLine);
        int endPos            = endBlock.position() + a.endChar;

        QTextCursor cursor(doc);
        cursor.setPosition(startPos);
        cursor.setPosition(endPos, QTextCursor::KeepAnchor);
        selections.cursor = cursor;

        selectionsList.append(selections);
    }
    this->setExtraSelections(selectionsList);
}



/**
跳转到指定的行
*/
void LosEditorUi::gotoLine(int line)
{
    QTextDocument *qtd = this->document();
    // line findBlockByLineNumber 可能对于 视觉行 会有问题
    QTextBlock block = qtd->findBlockByNumber(line);
    if (block.isValid())
    {
        QTextCursor cursor(qtd);
        cursor.setPosition(block.position());
        this->setTextCursor(cursor);
        // 定位到中间
        this->centerCursor();
        // 拿到焦点
        this->setFocus();
    }
}



void LosEditorUi::format()
{
    QString out{""};
    if (!LosCore::LosFormatManager::instance().format(&out, LOS_filePath->getFilePath(), toPlainText()))
    {
        return;
    }
    QTextCursor cur = textCursor();
    int outPos      = cur.position();
    cur.beginEditBlock();
    cur.select(QTextCursor::Document); // 全选
    cur.insertText(out);
    cur.endEditBlock();
    cur.setPosition(qMin(outPos, out.length()));
    this->setTextCursor(cur);
}



/**
导入内容
*/
void LosEditorUi::loadContextAndPath(LosModel::LosFileContext *context, LosModel::LosFilePath *file_path)
{
    if (!context || !file_path)
        return;
    LOS_context  = context;
    LOS_filePath = file_path;
    blockSignals(true);
    auto op      = LOS_context->load(LOS_filePath->getFilePath());
    QString text = "";
    if (op)
    {
        text = *op;
    }
    else
    {
        return;
    }
    setPlainText(text);
    blockSignals(false);
    emit LosCore::LosRouter::instance()._cmd_lsp_request_openFile(LOS_filePath -> getFilePath(), this->toPlainText());
}



/**
词汇补全
*/
void LosEditorUi::insertCompletion(const QString &completion)
{
    QTextCursor qtc = textCursor();
    int needLenth   = completion.size() - LOS_completer->completionPrefix().size();
    qtc.insertText(completion.right(needLenth));
    setTextCursor(qtc); // 为什么 这里还要 setTextCursor
}



/**
调用 LosContext 接口
*/
bool LosEditorUi::save()
{
    if (!LOS_context || !LOS_filePath)
        return false;
    bool ok = LOS_context->save(this->toPlainText(), LOS_filePath->getFilePath());
    if (ok)
    {
        L_dirty = false;
        emit LosCore::LosRouter::instance()._cmd_fileDirty(false);
        emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(), toPlainText());
    }
    return ok;
}



/**
get
获取当前光标下的单词
*/
QString LosEditorUi::getWordUnderCursor() const
{
    QTextCursor cursor = this->textCursor();
    QString text       = cursor.block().text();
    int col            = cursor.positionInBlock();
    int start          = col;
    while (start > 0)
    {
        QChar c = text.at(start - 1);
        if (!c.isLetterOrNumber() && c != '_')
        {
            break;
        }
        start--;
    }
    return text.mid(start, col - start);
}



bool LosEditorUi::isDirty() const
{
    return L_dirty;
}



/**
- 文字稍微 变动 就变脏
*/
void LosEditorUi::initConnect()
{
    L_timer = new QTimer(this);
    L_timer->setSingleShot(true);
    LOS_completer = new LosView::LosCompleterUi(this);
    LOS_completer->setWidget(this);
    LOS_highlighter = new LosCore::LosHighlighter(this->document());

    // activated 有两种
    connect(LOS_completer, QOverload<const QString &>::of(&QCompleter::activated), this,
            &LosEditorUi::insertCompletion);
    connect(this->document(), &QTextDocument::contentsChanged, this, &LosEditorUi::onTextChanged);
    connect(L_timer, &QTimer::timeout, this, &LosEditorUi::onDebounceTimeout);
    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_lsp_result_diagnostics, this,
            &LosEditorUi::showDiagnostic);
            
    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_lsp_result_completion, this,
            &LosEditorUi::showCompletion);

}


/**
- 初始化样式
*/
void LosEditorUi::initStyle()
{
    QFontMetrics met(this->font());
    int tab = 4 * met.horizontalAdvance(" ");
    this->setTabStopDistance(tab);
}



/**
- 变脏的信号
*/
void LosEditorUi::onTextChanged()
{
    if (!LOS_context)
        return;
    if (!L_dirty)
    {
        L_dirty = true;
        emit LosCore::LosRouter::instance()._cmd_fileDirty(true);
        emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(), toPlainText());
    }
    L_timer->start(200);
}



/**
- 防抖
*/
void LosEditorUi::onDebounceTimeout()
{
    QTextCursor cursor = this->textCursor();
    int line           = cursor.blockNumber();
    int col            = cursor.positionInBlock(); // 这个从 0 开始
    if (col == 0)
        return;
    QString currentLineText = cursor.block().text();
    QChar lastChar          = currentLineText.at(col - 1);
    bool isTriggerChar      = lastChar.isLetterOrNumber() || lastChar == '_' || lastChar == '.' || lastChar == '>' ||
                         lastChar == ':' || lastChar == '#' || lastChar == '/' || lastChar == '"' || lastChar == '<';
    if (!isTriggerChar)
        return;
    if (lastChar == '>' && (col < 2 || currentLineText.at(col - 2) != '-'))
        return;
    if (lastChar == ':' && (col < 2 || currentLineText.at(col - 2) != ':'))
        return;
    L_oldWord = getWordUnderCursor();
    emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(),
                                                                     this->toPlainText());
    emit LosCore::LosRouter::instance()._cmd_lsp_request_completeion(LOS_filePath -> getFilePath(), line, col);
}



/**
光标拦截
*/
void LosEditorUi::keyPressEvent(QKeyEvent *event)
{
    if (LOS_completer && LOS_completer->popup() && LOS_completer->popup()->isVisible())
    {
        switch (event->key())
        {
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



/**
按键拦截
*/
void LosEditorUi::mousePressEvent(QMouseEvent *event)
{
    // QApplication::keyboardModifiers()  获取当前所有 被 按住的键
    if (event->button() == Qt::LeftButton && (QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        QTextCursor cur = this->cursorForPosition(event->pos());
        int line        = cur.blockNumber();
        int col         = cur.positionInBlock();
        emit LosCore::LosRouter::instance()._cmd_whereDefine(line, col, LOS_filePath -> getFilePath());
        event->accept();
        return;
    }
    QPlainTextEdit::mousePressEvent(event);
}


/**
- 暂时先 监听 字体变化 然后修改 tab 按键的效果
*/
void LosEditorUi::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::FontChange)
    {
        QFontMetrics met(this->font());
        int tab = 4 * met.horizontalAdvance(" ");
        this->setTabStopDistance(tab);
    }
    QPlainTextEdit::changeEvent(e);
}
} // namespace LosView