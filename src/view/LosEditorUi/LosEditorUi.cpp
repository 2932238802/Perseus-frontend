#include "LosEditorUi.h"
#include "common/constants/ConstantsStr/LosEditorUiStr.h"

namespace LosView
{

    LosEditorUi::LosEditorUi(QWidget *parent) : QPlainTextEdit{parent}
    {
        initConnect();
        initStyle();
    }
    LosEditorUi::~LosEditorUi()
    {
        if (L_hoverPopup)
        {
            L_hoverPopup->deleteLater();
            L_hoverPopup = nullptr;
        }
    }



    /**
     * @brief showCompletion
     *
     * @param list
     */
    void LosEditorUi::showCompletion(const QStringList &list)
    {
        if (!this->hasFocus())
            return;
        if (list.empty() || !LOS_completer)
        {
            hideCompletionPopup();
            return;
        }
        LOS_completer->updateCompletionList(list);
        repositionCompletionPopup();
    }



    /*
     * 展示错误
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
            /*
             * 波浪线
             */
            format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
            QTextEdit::ExtraSelection selections;
            switch (a.ds)
            {
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Information:
            {
                format.setUnderlineColor(QColor("#8be9fd"));
                break;
            }
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Error:
            {
                format.setUnderlineColor(QColor("#ff5555"));
                break;
            }
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Hint:
            {
                format.setUnderlineColor(QColor("#6272a4"));
                break;
            }
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Warning:
            {
                format.setUnderlineColor(QColor("#ffb86c"));
                break;
            }
            default:
            {
                format.setUnderlineColor(QColor("#ff5555"));
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
        /*
         * this->setExtraSelections(selectionsList);
         */
        L_diagnosticSelections = selectionsList;
        highlightCurrentLine();
    }



    /**
     * @brief gotoLine
     *
     * @param in line
     */
    void LosEditorUi::gotoLine(int line)
    {
        QTextDocument *qtd = this->document();
        QTextBlock block   = qtd->findBlockByNumber(line);
        if (block.isValid())
        {
            QTextCursor cursor(qtd);
            cursor.setPosition(block.position());
            this->setTextCursor(cursor);
            this->centerCursor();
            this->setFocus();
        }
    }



    /**
     * @brief format 格式化
     *
     * if (out == currentText)
     * {
     *     return;
     * }
     * - 增加防抖
     * LOS_highlighter->updateSemanticTokens(QJsonArray{});
     * - 清楚 过期的 语义
     */
    void LosEditorUi::format()
    {
        QString out{""};
        QString currentText = toPlainText();
        if (LOS_filePath &&
            !LosCore::LosFormatManager::instance().format(&out, LOS_filePath->getFilePath(), toPlainText()))
        {
            return;
        }
        if (out == currentText)
        {
            return;
        }
        LOS_highlighter->updateSemanticTokens(QJsonArray{});
        QTextCursor cur = textCursor();
        int outPos      = cur.position();
        cur.beginEditBlock();
        cur.select(QTextCursor::Document);
        cur.insertText(out);
        cur.endEditBlock();
        cur.setPosition(qMin(outPos, out.length()));
        this->setTextCursor(cur);
    }



    /*
     * 导入内容
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
            this->document()->blockSignals(false);
            return;
        }
        /*
         * 加载新文件时必须：
         * 暂时关闭 undo 记录，避免把 setPlainText 本身计入撤销栈
         * setPlainText 之后清空历史，防止 Ctrl+Z 退回到旧文件的内容
         * 恢复 undo 记录
         */
        this->setUndoRedoEnabled(false);
        setPlainText(text);
        this->document()->clearUndoRedoStacks();
        this->setUndoRedoEnabled(true);
        this->document()->setModified(false);
        this->document()->blockSignals(false);
        L_dirty          = false;
        QString filePath = LOS_filePath->getFilePath();
        emit LosCore::LosRouter::instance()._cmd_lsp_request_openFile(filePath, this -> toPlainText());
    }



    /*
     * 词汇补全
     * - 用 editBlock 包住，保证 Ctrl+Z 能一次撤回整个补全
     * - 先选中已经输入的 prefix，再替换成完整 completion，这样对于
     *   大小写不一致 / prefix 是 completion 的不严格前缀 的情况也能正确工作
     */
    void LosEditorUi::insertCompletion(const QString &completion)
    {
        if (!LOS_completer)
            return;
        QTextCursor qtc     = textCursor();
        const int prefixLen = LOS_completer->completionPrefix().size();
        qtc.beginEditBlock();
        if (prefixLen > 0)
        {
            qtc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, prefixLen);
        }
        qtc.insertText(completion);
        qtc.endEditBlock();
        setTextCursor(qtc);
        hideCompletionPopup();
    }



    /**
     * @brief lineNumberAreaPaintEvent
     *
     * @param event
     */
    void LosEditorUi::lineNumberAreaPaintEvent(QPaintEvent *event)
    {
        QPainter painter(LOS_lineNumber);
        painter.fillRect(event->rect(), QColor("#21222c"));
        QTextBlock block = firstVisibleBlock();
        int blockNumber  = block.blockNumber();

        /*
         * qRound 是 四舍五入
         * blockBoundingGeometry QTextBlock
         * 获取 当前 textblock的边界框
         */
        int top    = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
        int bottom = top + qRound(blockBoundingRect(block).height());

        /*
         * event->rect().bottom()
         * 当前要画的 矩形 的 最下边 如果算出来的比 top 要小
         * 就不用画了
         */
        while (block.isValid() && top <= event->rect().bottom())
        {
            if (block.isVisible() && bottom >= event->rect().top())
            {
                /*
                 * 将内部从 0 开始的索引，
                 * 转成人类习惯的从 1 开始的字符串
                 */
                QString number = QString::number(blockNumber + 1);

                /*
                 * 准备颜料（高亮当前行）
                 * 就是 当前 这一行的数据
                 */
                if (textCursor().blockNumber() == blockNumber)
                {
                    painter.setPen(QColor(LosCommon::LosEditorUi_Constants::LINENUMBER_CURLINE));
                }
                else
                {
                    painter.setPen(QColor(LosCommon::LosEditorUi_Constants::LINENUMBER_UNCURLINE));
                }

                /*
                 * x坐标, y坐标, 宽度, 高度,
                 * 对齐方式, 要写的字
                 */
                painter.drawText(0, top, LOS_lineNumber->width() - 5, fontMetrics().height(),
                                 Qt::AlignRight | Qt::AlignVCenter, number);
            }
            block  = block.next();
            top    = bottom;
            bottom = top + qRound(blockBoundingRect(block).height());
            ++blockNumber;
        }
    }



    /**
     * @brief save 保存当前文件
     *
     * @return true
     * @return false
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
            this->document()->setModified(false);
            L_dirty = false;
            emit LosCore::LosRouter::instance()._cmd_fileDirty(LOS_filePath -> getFilePath(), false);
            emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(),
                                                                             toPlainText());
        }
        return ok;
    }



    /**
     * @brief getWordUnderCursor 获取当前光标下的单词
     *
     * @return QString
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



    /**
     * @brief isDirty 是否为脏文件
     *
     * @return true
     * @return false
     */
    bool LosEditorUi::isDirty() const
    {
        return L_dirty;
    }



    /**
     * @brief getLineNumberWidth
     *
     * @return int
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



    /*
     * - 文字稍微 变动 就变脏
     * - 绑定 悬停效果
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
        connect(&router, &LosCore::LosRouter::_cmd_openFile_suc, this, &LosEditorUi::onOpenFileSuc);
    }



    /**
     * @brief initStyle
     * - 设置样式
     * - 设置鼠标 追踪
     */
    void LosEditorUi::initStyle()
    {
        QFontMetrics met(this->font());
        int tab = 4 * met.horizontalAdvance(" ");
        this->setTabStopDistance(tab);
        updateLineNumberAreaWidth();
        viewport()->setMouseTracking(true);
        setMouseTracking(true);
    }



    /**
     * @brief updateLineNumberArea 更新 左侧的数
     *
     * @param in rect
     * @param in dy
     */
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
            updateLineNumberAreaWidth();
        }
    }



    /**
     * @brief updateLineNumberAreaWidth
     * 更新左侧的宽度
     */
    void LosEditorUi::updateLineNumberAreaWidth()
    {
        /*
         * 左、上、右、下
         * getLineNumberWidth 获取一个 左侧的长度
         */
        setViewportMargins(getLineNumberWidth(), 0, 0, 0);
    }



    /**
     * @brief highlightCurrentLine 高亮行
     *
     */
    void LosEditorUi::highlightCurrentLine()
    {
        QList<QTextEdit::ExtraSelection> extra;
        extra.append(L_diagnosticSelections);
        if (!isReadOnly())
        {
            QTextEdit::ExtraSelection selection;
            QColor lineColor = QColor("#44475a");
            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extra.append(selection);
        }
        extra.append(L_hoverSelections);
        setExtraSelections(extra);
    }



    /**
     * @brief updateHoverUnderline
     * 鼠标ctrl 悬停 下划线高亮
     *
     * @param in vpPos 全局位置
     */
    void LosEditorUi::updateHoverUnderline(const QPoint &vpPos)
    {
        QTextCursor cursor = cursorForPosition(vpPos);
        cursor.select(QTextCursor::WordUnderCursor);
        QString word = cursor.selectedText();
        if (word.isEmpty() || (!word.at(0).isLetterOrNumber() && word.at(0) != '_'))
        {
            clearHoverUnderline();
            viewport()->setCursor(Qt::IBeamCursor);
            return;
        }

        if (L_lastCursor.selectionStart() == cursor.selectionStart() &&
            L_lastCursor.selectionEnd() == cursor.selectionEnd())
        {
            return;
        }

        L_lastCursor = cursor;
        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format.setFontUnderline(true);
        sel.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        sel.format.setForeground(QColor("#8be9fd"));
        L_hoverSelections = {sel};
        highlightCurrentLine();
        viewport()->setCursor(Qt::PointingHandCursor);
    }



    /**
     * @brief clearHoverUnderline 清理下划线
     *
     */
    void LosEditorUi::clearHoverUnderline()
    {
        L_hoverSelections.clear();
        L_lastCursor = QTextCursor();
        highlightCurrentLine();
    }


    /**
     * @brief hideCompletionPopup
     *
     */
    void LosEditorUi::hideCompletionPopup()
    {
        if (LOS_completer && LOS_completer->popup())
        {
            LOS_completer->popup()->hide();
        }
        L_showComplete = false;
    }



    /**
     * @brief repositionCompletionPopup 重新定位提示词位置
     *
     * @return true
     * @return false
     */
    bool LosEditorUi::repositionCompletionPopup()
    {
        if (!LOS_completer)
            return false;
        QString prefix = getWordUnderCursor();
        if (prefix != LOS_completer->completionPrefix())
        {
            LOS_completer->setCompletionPrefix(prefix);
        }

        if (LOS_completer->completionCount() == 0)
        {
            hideCompletionPopup();
            return false;
        }

        QRect r = cursorRect();
        r.translate(viewport()->pos());

        if (!prefix.isEmpty())
        {
            QFontMetrics fm(this->fontMetrics());
            int prefixPixelWidth = fm.horizontalAdvance(prefix);
            r.translate(-prefixPixelWidth, 0);
        }

        int idealWidth          = LOS_completer->popup()->sizeHintForColumn(0);
        constexpr int PADDING   = 25;
        constexpr int MAX_WIDTH = 500;
        r.setWidth(qMin(idealWidth + PADDING, MAX_WIDTH));

        LOS_completer->complete(r);
        L_showComplete = true;
        return true;
    }



    /**
     * @brief onTextChanged
     * - 变脏的信号
     * - 修复 防止 抖动的逻辑
     *
     */
    void LosEditorUi::onTextChanged()
    {
        if (!LOS_context)
            return;
        if (!L_dirty && LOS_filePath && this->document()->isModified())
        {
            L_dirty          = true;
            QString filePath = LOS_filePath->getFilePath();
            emit LosCore::LosRouter::instance()._cmd_fileDirty(filePath, true);
            // 统一给timer处理
            // emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(filePath, this -> toPlainText());
            emit LosCore::LosRouter::instance()._cmd_lsp_request_semantic(filePath);
        }
        L_timer->start(200);
    }



    /**
     * @brief onDebounceTimeout 防抖语法补全
     */
    void LosEditorUi::onDebounceTimeout()
    {
        if (!LOS_filePath)
            return;
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
     * @brief
     *
     * @param markdownContent
     */
    void LosEditorUi::onHover_Clangd(const QString &markdownContent)
    {
        if (markdownContent.isEmpty())
        {
            hideHoverPopup();
            return;
        }

        QString html = markdownContent;
        html.replace("<", "&lt;");
        html.replace(">", "&gt;");
        html.replace("```cpp\n", "<pre style='color:#8be9fd; font-family:Consolas; margin: 5px 0;'>");
        html.replace("```c\n", "<pre style='color:#8be9fd; font-family:Consolas; margin: 5px 0;'>");
        html.replace("```", "</pre>");
        QRegularExpression boldRegex("\\*\\*(.*?)\\*\\*");

        html.replace(boldRegex, "<b>\\1</b>");
        QRegularExpression inlineCodeRegex("`([^`]+)`");
        html.replace(inlineCodeRegex, "<code style='color:#f1fa8c; background-color:#44475a; padding:2px 4px; "
                                      "border-radius:3px;'>\\1</code>");
        html.replace("\n", "<br>");

        showHoverPopup(html);
    }


    /**
     * @brief showHoverPopup 显示自绘 hover 浮窗
     *
     * @param html 已转换为 rich text 的 hover 内容
     */
    void LosEditorUi::showHoverPopup(const QString &html)
    {
        /*
         * 用一个顶层 Tool 窗口做浮窗, 不继承 parent 的焦点语义,
         * 也不被 Qt 的 QToolTip 管理器影响
         */
        if (!L_hoverPopup)
        {
            L_hoverPopup = new QLabel(nullptr, Qt::ToolTip | Qt::FramelessWindowHint);
            L_hoverPopup->setTextFormat(Qt::RichText);
            L_hoverPopup->setTextInteractionFlags(Qt::NoTextInteraction);
            L_hoverPopup->setWordWrap(true);
            L_hoverPopup->setMargin(8);
            L_hoverPopup->setMaximumWidth(600);
            L_hoverPopup->setAttribute(Qt::WA_ShowWithoutActivating, true);
            L_hoverPopup->setFocusPolicy(Qt::NoFocus);
            L_hoverPopup->setStyleSheet(QStringLiteral(R"(
                QLabel {
                    background-color: #21222c;
                    color: #f8f8f2;
                    border: 1px solid #bd93f9;
                    border-radius: 6px;
                    padding: 6px 10px;
                    font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
                    font-size: 13px;
                }
            )"));
        }

        L_hoverPopup->setText(html);
        L_hoverPopup->adjustSize();
        QPoint anchor = L_lastHoverWordRectGlobal.isValid() ? L_lastHoverWordRectGlobal.bottomLeft()
                                                            : L_lastHoverGlobal + QPoint(0, 20);
        anchor += QPoint(0, 4);

        QScreen *screen = QGuiApplication::screenAt(anchor);
        if (!screen)
            screen = QGuiApplication::primaryScreen();
        QRect available = screen ? screen->availableGeometry() : QRect(0, 0, 1920, 1080);
        QSize popupSize = L_hoverPopup->sizeHint();

        if (anchor.x() + popupSize.width() > available.right())
            anchor.setX(available.right() - popupSize.width() - 4);
        if (anchor.x() < available.left() + 4)
            anchor.setX(available.left() + 4);
        if (anchor.y() + popupSize.height() > available.bottom())
        {
            int above = L_lastHoverWordRectGlobal.top() - popupSize.height() - 4;
            anchor.setY(qMax(above, available.top() + 4));
        }

        L_hoverPopup->move(anchor);
        L_hoverPopup->show();
        L_hoverPopup->raise();
    }


    /**
     * @brief hideHoverPopup 隐藏自绘 hover 浮窗
     */
    void LosEditorUi::hideHoverPopup()
    {
        if (L_hoverPopup && L_hoverPopup->isVisible())
            L_hoverPopup->hide();
        L_lastHoverWord.clear();
    }



    /**
     * @brief onSemanticLegend
     *
     * @param token_types
     * @param legend_token_modifiers
     */
    void LosEditorUi::onSemanticLegend(const QStringList &token_types, const QStringList &legend_token_modifiers)
    {
        LOS_highlighter->initSemanticLegend(token_types, legend_token_modifiers);
    }



    /**
     * @brief onSemanticTokens
     *
     * @param data
     */
    void LosEditorUi::onSemanticTokens(const QJsonArray &data)
    {
        LOS_highlighter->updateSemanticTokens(data);
    }



    /**
     * @brief onCutKeyClicked
     * 剪切一行
     */
    void LosEditorUi::onCutKeyClicked()
    {
        QTextCursor cursor = textCursor();
        if (!cursor.hasSelection())
        {
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            if (!cursor.atEnd())
            {
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            }
            setTextCursor(cursor);
            cut();
            return;
        }
    }



    /**
     * @brief onCopyCurrentLine
     * 复制当前行
     */
    void LosEditorUi::onCopyCurrentLine()
    {
        QTextCursor cursor = textCursor();
        if (!cursor.hasSelection())
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
            return;
        }
    }



    /**
     * @brief onControlKeyClicked
     * - ctrl + 鼠标左键
     * - 高亮
     *
     * @param in event
     * - 鼠标事件
     */
    void LosEditorUi::onControlKeyPressed()
    {
        L_ctrlBtnPresses   = true;
        const QPoint vpPos = viewport()->mapFromGlobal(QCursor::pos());
        if (viewport()->rect().contains(vpPos))
        {
            updateHoverUnderline(vpPos);
        }
    }



    /**
     * @brief onOpenFileSuc 异步请求语法高亮
     *
     * @param filePath
     */
    void LosEditorUi::onOpenFileSuc(const QString &filePath)
    {
        if (filePath == LOS_filePath->getAbsoluteFilePath())
        {
            emit LosCore::LosRouter::instance()._cmd_lsp_request_semantic(filePath);
        }
    }



    /*
     * 光标拦截
     * - 弹出 语法补全
     * - 同时 支持 括号补全
     */
    void LosEditorUi::keyPressEvent(QKeyEvent *event)
    {
        hideHoverPopup();
        if (event->key() == Qt::Key_Control)
        {
            onControlKeyPressed();
        }

        if (LOS_completer && LOS_completer->popup() && LOS_completer->popup()->isVisible())
        {
            switch (event->key())
            {
            case Qt::Key_Escape:
                hideCompletionPopup();
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
            onCutKeyClicked();
        if (event->matches(QKeySequence::Copy))
            onCopyCurrentLine();

        static const QHash<QChar, QChar> AUTO_CLOSE_MAP{{'{', '}'}, {'[', ']'}, {'(', ')'}};
        if (!event->text().isEmpty())
        {
            QChar inputChar = event->text().at(0);
            if (AUTO_CLOSE_MAP.contains(inputChar))
            {
                /*
                 * 把左右括号合并到同一个 editBlock 中，使 Ctrl+Z 一次就能
                 * 撤销整对括号，而不是先撤右再撤左
                 */
                QTextCursor cursor = textCursor();
                cursor.beginEditBlock();
                cursor.insertText(QString(inputChar));
                cursor.insertText(QString(AUTO_CLOSE_MAP[inputChar]));
                cursor.movePosition(QTextCursor::PreviousCharacter);
                cursor.endEditBlock();
                setTextCursor(cursor);
                return;
            }
        }
        QPlainTextEdit::keyPressEvent(event);
        if (L_showComplete && LOS_completer && LOS_completer->popup()->isVisible())
        {
            repositionCompletionPopup();
        }
    }



    /**
     * @brief keyReleaseEvent
     *
     * @param event
     */
    void LosEditorUi::keyReleaseEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Control)
        {
            L_ctrlBtnPresses = false;
            clearHoverUnderline();
            viewport()->setCursor(Qt::IBeamCursor);
        }
        QPlainTextEdit::keyReleaseEvent(event);
    }



    /**
     * @brief mousePressEvent
     *
     * @param event
     */
    void LosEditorUi::mousePressEvent(QMouseEvent *event)
    {

        if (!LOS_filePath)
            return;
        hideCompletionPopup();
        hideHoverPopup();
        if (event->button() == Qt::LeftButton && (QApplication::keyboardModifiers() & Qt::ControlModifier))
        {
            QTextCursor cur = this->cursorForPosition(event->pos());
            int line        = cur.blockNumber();
            int col         = cur.positionInBlock();
            emit LosCore::LosRouter::instance()._cmd_whereDefine(line, col, LOS_filePath -> getFilePath());
            clearHoverUnderline();
            viewport()->setCursor(Qt::IBeamCursor);
            event->accept();
            return;
        }
        QPlainTextEdit::mousePressEvent(event);
    }



    /**
     * @brief mouseMoveEvent
     *
     * @param event
     */
    void LosEditorUi::mouseMoveEvent(QMouseEvent *event)
    {
        if (L_ctrlBtnPresses)
        {
            updateHoverUnderline(event->pos());
        }
        /*
         * 鼠标离开当前悬停的单词矩形则隐藏 hover 浮窗
         */
        if (L_hoverPopup && L_hoverPopup->isVisible() && L_lastHoverWordRectGlobal.isValid())
        {
            QPoint g = event->globalPosition().toPoint();
            if (!L_lastHoverWordRectGlobal.contains(g))
            {
                hideHoverPopup();
            }
        }
        QPlainTextEdit::mouseMoveEvent(event);
    }



    /**
     * @brief leaveEvent
     *
     * @param event
     */
    void LosEditorUi::leaveEvent(QEvent *event)
    {
        clearHoverUnderline();
        viewport()->setCursor(Qt::IBeamCursor);
        hideHoverPopup();
        QPlainTextEdit::leaveEvent(event);
    }



    /**
     * @brief changeEvent
     *
     * @param e
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
     * @brief event 鼠标悬停的效果
     *
     * @param event
     * @return true
     * @return false
     */
    bool LosEditorUi::event(QEvent *event)
    {
        if (!LOS_filePath)
            return QPlainTextEdit::event(event);
        if (event->type() == QEvent::ToolTip)
        {
            QHelpEvent *help = static_cast<QHelpEvent *>(event);

            /*
             * help->pos() 来自 viewport 事件后被 Qt 转成 widget 坐标,
             * 对于 QPlainTextEdit, cursorForPosition 接受的是 viewport 坐标,
             * 所以需要减去 viewport()->pos()
             */
            QPoint vpPos       = help->pos() - viewport()->pos();
            QTextCursor cursor = cursorForPosition(vpPos);

            QTextCursor wordCursor = cursor;
            wordCursor.select(QTextCursor::WordUnderCursor);
            QString word = wordCursor.selectedText();

            /*
             * 不在单词上 (空白/标点) 不触发 hover
             */
            if (word.isEmpty() || (!word.at(0).isLetterOrNumber() && word.at(0) != '_'))
            {
                hideHoverPopup();
                return QPlainTextEdit::event(event);
            }

            QTextCursor s = wordCursor;
            s.setPosition(wordCursor.selectionStart());
            QTextCursor e = wordCursor;
            e.setPosition(wordCursor.selectionEnd());
            QRect rs                  = cursorRect(s);
            QRect re                  = cursorRect(e);
            QRect localRect           = QRect(rs.topLeft(), re.bottomRight()).translated(viewport()->pos());
            L_lastHoverWordRectGlobal = QRect(mapToGlobal(localRect.topLeft()), localRect.size());
            L_lastHoverGlobal         = help->globalPos();

            if (word == L_lastHoverWord && L_hoverPopup && L_hoverPopup->isVisible())
            {
                return QPlainTextEdit::event(event);
            }
            L_lastHoverWord = word;

            int line = cursor.blockNumber();
            int col  = cursor.positionInBlock();
            emit LosCore::LosRouter::instance()._cmd_lsp_request_hover(LOS_filePath -> getFilePath(), line, col);
            return true;
        }
        return QPlainTextEdit::event(event);
    }



    /**
     * @brief resizeEvent
     *
     * @param in e
     */
    void LosEditorUi::resizeEvent(QResizeEvent *e)
    {
        QRect cr = contentsRect();
        LOS_lineNumber->setGeometry(QRect(cr.left(), cr.top(), getLineNumberWidth(), cr.height()));
        QPlainTextEdit::resizeEvent(e);
    }



    /**
     * @brief focusOutEvent
     *
     * @param event
     */
    void LosEditorUi::focusOutEvent(QFocusEvent *event)
    {
        L_ctrlBtnPresses = false;
        clearHoverUnderline();
        viewport()->setCursor(Qt::IBeamCursor);
        hideHoverPopup();
        QPlainTextEdit::focusOutEvent(event);
    }
} /* namespace LosView */