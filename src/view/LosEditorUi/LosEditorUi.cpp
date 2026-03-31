#include "LosEditorUi.h"
#include "core/LosRouter/LosRouter.h"
#include <algorithm>
#include <qcoreevent.h>
#include <qevent.h>
#include <qkeysequence.h>
#include <qplaintextedit.h>
#include <qtextcursor.h>
#include <qtooltip.h>



namespace LosView
{

LosEditorUi::LosEditorUi(QWidget *parent) : QPlainTextEdit{parent}
{
    initConnect();
    initStyle();
}
LosEditorUi::~LosEditorUi() {}



/**
展示弹窗
*/
/**
展示弹窗
*/
void LosEditorUi::showCompletion(const QStringList &list)
{
    if (!this->hasFocus())
        return;

    if (list.empty() || nullptr == LOS_completer)
    {
        if (LOS_completer && LOS_completer->popup())
        {
            LOS_completer->popup()->hide();
        }
        return;
    }
    LOS_completer->updateCompletionList(list);
    QString prefix = getWordUnderCursor();
    if (prefix != LOS_completer->completionPrefix())
    {
        LOS_completer->setCompletionPrefix(prefix);
    }
    QRect r = cursorRect();
    QFontMetrics fm(this->font());
    int prefixPixelWidth = fm.horizontalAdvance(prefix);
    r.translate(-prefixPixelWidth, 0);
    int idealWidth = LOS_completer->popup()->sizeHintForColumn(0);
    int padding    = 25;
    int finalWidth = qMin(idealWidth + padding, 500);
    r.setWidth(finalWidth);
    LOS_completer->complete(r);
    L_showComplete = true;
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
void LosEditorUi::loadContextAndPath(QSharedPointer<LosModel::LosFileContext> context,
                                     QSharedPointer<LosModel::LosFilePath> file_path)
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
    L_showComplete = false;
}



/**
调用 LosContext 接口
*/
bool LosEditorUi::save()
{
    if (LOS_context.isNull() || LOS_filePath.isNull())
        return false;
    QString filePath = LOS_filePath->getFilePath();
    if (filePath.isEmpty())
        return false;
    bool ok = LOS_context->save(this->toPlainText(), filePath);
    if (ok)
    {
        // 保存的时候 自动格式化
        format();
        L_dirty = false;
        emit LosCore::LosRouter::instance()._cmd_fileDirty(LOS_filePath -> getFilePath(), false);

        // 保存的时候 还要 检查一下 是不是
        // diag 报错机制 再次显示
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
- 绑定 悬停效果
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
    connect(&LosCore::LosRouter::instance(), &LosCore::LosRouter::_cmd_lsp_result_hover, this,
            &LosEditorUi::onHover_Clangd);
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
- 自定义 工具 剪切
*/
void LosEditorUi::cutCurrentLine()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    if (!cursor.atEnd())
    {
        // 吃掉 后面的 \n
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    }
    setTextCursor(cursor);
    cut();
}



/**
- 复制 一行
*/
void LosEditorUi::copyCurrentLine()
{
    QTextCursor cursor         = textCursor();
    QTextCursor originalCursor = cursor;
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    if (!cursor.atEnd())
    {
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    }
    setTextCursor(cursor);
    copy();
    setTextCursor(originalCursor);
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
        emit LosCore::LosRouter::instance()._cmd_fileDirty(LOS_filePath -> getFilePath(), true);
        emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(), toPlainText());
    }
    LOS_completer->popup()->hide();
    L_showComplete = false;
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

    // 获取 上一个 字符
    L_oldWord = getWordUnderCursor();
    emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(),
                                                                     this->toPlainText());
    emit LosCore::LosRouter::instance()._cmd_lsp_request_completeion(LOS_filePath -> getFilePath(), line, col);
}



/**
- clangd 语法补全
*/
void LosEditorUi::onHover_Clangd(const QString &markdownContent)
{
    if (markdownContent.isEmpty())
    {
        QToolTip::hideText();
        return;
    }
    QString html = markdownContent;
    html.replace("```cpp", "<pre style='color:#569cd6; font-family:Consolas;'>");
    html.replace("```c", "<pre style='color:#569cd6; font-family:Consolas;'>");
    html.replace("```", "</pre>");
    html.replace("\n", "<br>");
    html.replace("**", "<b>");
    QToolTip::showText(L_lastHoverGlobal, html, this);
}



/**
- 光标拦截
    - 弹出 语法补全
- 同时 支持 括号补全
*/
// 在 LosEditorUi.cpp 中
void LosEditorUi::keyPressEvent(QKeyEvent *event)
{
    if (LOS_completer && LOS_completer->popup() && LOS_completer->popup()->isVisible())
    {
        switch (event->key())
        {
        case Qt::Key_Escape:
            LOS_completer->popup()->hide();
            L_showComplete = false;
            event->ignore();
            return;
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            event->ignore();
            return;
        default:
            break;
        }
    }

    if (event->matches(QKeySequence::Cut))
    {
        QTextCursor cursor = textCursor();
        if (!cursor.hasSelection())
        {
            cutCurrentLine();
            return;
        }
    }

    if (event->matches(QKeySequence::Copy))
    {
        QTextCursor cursor = textCursor();
        if (!cursor.hasSelection())
        {
            copyCurrentLine();
            return;
        }
    }

    static const QHash<QChar, QChar> AUTO_CLOSE_MAP{{'{', '}'}, {'[', ']'}, {'(', ')'}};
    if (!event->text().isEmpty())
    {
        QChar inputChar = event->text().at(0);
        if (AUTO_CLOSE_MAP.contains(inputChar))
        {
            QPlainTextEdit::keyPressEvent(event);

            QTextCursor cursor = textCursor();
            cursor.insertText(AUTO_CLOSE_MAP[inputChar]);

            cursor.movePosition(QTextCursor::PreviousCharacter);
            setTextCursor(cursor);

            return;
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



/**
- 鼠标悬停的效果
*/
bool LosEditorUi::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent *help   = static_cast<QHelpEvent *>(event);
        QTextCursor cursor = cursorForPosition(help->pos());
        int line           = cursor.blockNumber();
        int col            = cursor.positionInBlock();
        L_lastHoverGlobal  = help->globalPos();
        emit LosCore::LosRouter::instance()._cmd_lsp_request_hover(LOS_filePath -> getFilePath(), line, col);
        return true;
    }
    return QPlainTextEdit::event(event);
}

} // namespace LosView