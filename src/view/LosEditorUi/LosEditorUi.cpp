#include "LosEditorUi.h"
#include "view/LosLineNumberUi/LosLineNumberUi.h"
#include <qglobal.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <qtextobject.h>



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
        if (LOS_filePath && file_path != LOS_filePath->getFilePath())
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
        // this->setExtraSelections(selectionsList);
        L_diagnosticSelections = selectionsList;
        highlightCurrentLine();
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



    /**
    if (out == currentText)
        {
            return;
        }
    - 增加防抖
    */
    void LosEditorUi::format()
    {
        QString out{""};
        QString currentText = toPlainText();
        if (!LosCore::LosFormatManager::instance().format(&out, LOS_filePath->getFilePath(), toPlainText()))
        {
            return;
        }
        // fix
        if (out == currentText)
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
        this->document()->blockSignals(true);
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
        this->document()->blockSignals(false);
        QString filePath = LOS_filePath->getFilePath();
        emit LosCore::LosRouter::instance()._cmd_lsp_request_openFile(filePath, this -> toPlainText());
        emit LosCore::LosRouter::instance()._cmd_lsp_request_semantic(filePath);
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



    void LosEditorUi::lineNumberAreaPaintEvent(QPaintEvent *event)
    {
        QPainter painter(LOS_lineNumber);
        painter.fillRect(event->rect(), QColor("#181825"));

        QTextBlock block = firstVisibleBlock(); // 第一个 编辑区域
        int blockNumber  = block.blockNumber();

        // qRound 是 四舍五入
        // blockBoundingGeometry QTextBlock
        // 获取 当前 textblock的边界框
        int top    = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
        int bottom = top + qRound(blockBoundingRect(block).height());

        // event->rect().bottom()
        // 当前要画的 矩形 的 最下边 如果算出来的比 top 要小
        // 就不用画了
        while (block.isValid() && top <= event->rect().bottom())
        {
            if (block.isVisible() && bottom >= event->rect().top())
            {
                // 将内部从 0 开始的索引，
                // 转成人类习惯的从 1 开始的字符串
                QString number = QString::number(blockNumber + 1);

                // 准备颜料（高亮当前行）
                // 就是 当前 这一行的数据
                if (textCursor().blockNumber() == blockNumber)
                {
                    painter.setPen(QColor("#cdd6f4"));
                }
                else
                {
                    painter.setPen(QColor("#6c7086"));
                }

                // x坐标, y坐标, 宽度, 高度,
                // 对齐方式, 要写的字
                painter.drawText(0, top, LOS_lineNumber->width() - 5, fontMetrics().height(),
                                 Qt::AlignRight | Qt::AlignVCenter, number);
            }

            // 准备画下一行
            // 推进链表和坐标）
            block = block.next();
            top   = bottom;
            //
            bottom = top + qRound(blockBoundingRect(block).height());
            ++blockNumber;
        }
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
        format();
        bool ok = LOS_context->save(this->toPlainText(), filePath);
        if (ok)
        {
            // 保存的时候 自动格式化
            this->document()->setModified(false);
            L_dirty = false;
            emit LosCore::LosRouter::instance()._cmd_fileDirty(LOS_filePath -> getFilePath(), false);

            // 保存的时候 还要 检查一下 是不是
            // diag 报错机制 再次显示
            emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(),
                                                                             toPlainText());
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
    - 获取 尺寸 宽度
    */
    int LosEditorUi::getLineNumberWidth() const
    {
        int digit = 1;
        int max   = qMax(1, blockCount());
        while (max >= 10)
        {
            max /= 10;
            digit++;
        }
        int space = LosCommon::LosLineNumberUi_Constants::BASE_LINEWIDTH +
                    fontMetrics().horizontalAdvance(QLatin1Char('9')) * digit;
        return space;
    }



    /**
    - 文字稍微 变动 就变脏
    - 绑定 悬停效果
    */
    void LosEditorUi::initConnect()
    {
        L_timer = new QTimer(this);
        L_timer->setSingleShot(true);

        // 语法补全的弹窗
        LOS_completer = new LosView::LosCompleterUi(this);
        LOS_completer->setWidget(this);

        // 语法高亮
        LOS_highlighter = new LosCore::LosHighlighter(this->document());

        // 行 号
        LOS_lineNumber = new LosView::LosLineNumberUi(this);

        // activated 有两种
        auto &router = LosCore::LosRouter::instance();
        connect(LOS_completer, QOverload<const QString &>::of(&QCompleter::activated), this,
                &LosEditorUi::insertCompletion);
        connect(this->document(), &QTextDocument::contentsChanged, this, &LosEditorUi::onTextChanged);
        connect(L_timer, &QTimer::timeout, this, &LosEditorUi::onDebounceTimeout);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_diagnostics, this, &LosEditorUi::showDiagnostic);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_completion, this, &LosEditorUi::showCompletion);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_hover, this, &LosEditorUi::onHover_Clangd);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_semanticLegend, this, &LosEditorUi::onSemanticLegend);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_semanticTokens, this, &LosEditorUi::onSemanticTokens);
        connect(this, &LosEditorUi::blockCountChanged, this, &LosEditorUi::updateLineNumberAreaWidth);
        connect(this, &LosEditorUi::updateRequest, this, &LosEditorUi::updateLineNumberArea);
        connect(this, &LosEditorUi::cursorPositionChanged, this,
                [this]()
                {
                    LOS_lineNumber->update();
                    highlightCurrentLine();
                });
    }



    /**
    - 初始化样式
    */
    void LosEditorUi::initStyle()
    {
        QFontMetrics met(this->font());
        int tab = 4 * met.horizontalAdvance(" ");
        this->setTabStopDistance(tab);
        updateLineNumberAreaWidth(0);
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



    void LosEditorUi::updateLineNumberArea(const QRect &rect, int dy)
    {
        if (dy)
        {
            LOS_lineNumber->scroll(0, dy);
        }
        else
        {
            LOS_lineNumber->update(0, rect.y(), LOS_lineNumber->width(), rect.height());
        }
        if (rect.contains(viewport()->rect()))
        {
            updateLineNumberAreaWidth(0);
        }
    }



    void LosEditorUi::updateLineNumberAreaWidth(int)
    {
        // 左、上、右、下
        // getLineNumberWidth 获取一个 左侧的长度
        setViewportMargins(getLineNumberWidth(), 0, 0, 0);
    }



    void LosEditorUi::highlightCurrentLine()
    {
        QList<QTextEdit::ExtraSelection> extra = L_diagnosticSelections;
        if (isReadOnly())
            return;
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor("#313244");
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extra.append(selection);
        setExtraSelections(extra);
    }



    /**
    - 变脏的信号
    - 修复 防止 抖动的逻辑
    */
    void LosEditorUi::onTextChanged()
    {
        if (!LOS_context)
            return;
        if (!L_dirty && this->document()->isModified())
        {
            L_dirty          = true;
            QString filePath = LOS_filePath->getFilePath();
            emit LosCore::LosRouter::instance()._cmd_fileDirty(filePath, true);
            emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(filePath, this -> toPlainText());
            emit LosCore::LosRouter::instance()._cmd_lsp_request_semantic(filePath);
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
        int col            = cursor.positionInBlock();
        if (col == 0)
            return;
        QString currentLineText = cursor.block().text();
        QChar lastChar          = currentLineText.at(col - 1);
        bool isTriggerChar = lastChar.isLetterOrNumber() || lastChar == '_' || lastChar == '.' || lastChar == '>' ||
                             lastChar == ':' || lastChar == '#' || lastChar == '/' || lastChar == '"' ||
                             lastChar == '<';
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
    - 更新 初始化 样式表
    */
    void LosEditorUi::onSemanticLegend(const QStringList &token_types, const QStringList &legend_token_modifiers)
    {
        LOS_highlighter->initSemanticLegend(token_types, legend_token_modifiers);
    }



    /**
    - 更新 颜色
    */
    void LosEditorUi::onSemanticTokens(const QJsonArray &data)
    {
        LOS_highlighter->updateSemanticTokens(data);
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
        if (LOS_completer && LOS_completer->popup())
        {
            LOS_completer->popup()->hide();
        }
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



    void LosEditorUi::resizeEvent(QResizeEvent *e)
    {
        QRect cr = contentsRect();
        LOS_lineNumber->setGeometry(QRect(cr.left(), cr.top(), getLineNumberWidth(), cr.height()));
        QPlainTextEdit::resizeEvent(e);
    }


} // namespace LosView