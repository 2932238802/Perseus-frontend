// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosEditorUi.h"
#include "common/constants/ConstantsClass/LosCodeFoldingModelClass.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "common/constants/ConstantsNum/LosCompleterUiNum.h"
#include "common/constants/ConstantsNum/LosEditorUiNum.h"
#include "common/constants/ConstantsNum/LosLineNumberUiNum.h"
#include "common/constants/ConstantsStr/LosEditorUiStr.h"
#include "common/util/CheckLang.h"
#include "common/util/FindMatchBracket.h"
#include "common/util/GetLeadingWhiteSpace.h"
#include "core/LosFinder/LosFinder.h"
#include "core/LosFormat/LosFormatManager/LosFormatManager.h"
#include "core/LosHighlighter/LosHighlighter.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosTree/LosTreeSitterDocument/LosTreeSitterDocument.h"
#include "core/LosTree/LosTreeSitterFoldRange/LosTreeSitterFoldRange.h"
#include "core/LosTree/LosTreeSitterFoldingProvider/LosTreeSitterFoldingProvider.h"
#include "models/LosCodeFoldingModel/LosCodeFoldingModel.h"
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosCompleterUi/LosCompleterUi.h"
#include "view/LosHoverPopup/LosHoverPopup.h"
#include "view/LosLineNumberUi/LosLineNumberUi.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QCompleter>
#include <QDebug>
#include <QFileInfo>
#include <QGuiApplication>
#include <QObject>
#include <QPainter>
#include <QScreen>
#include <QScrollBar>
#include <QTextBlock>
#include <QToolTip>
#include <algorithm>
#include <cfloat>
#include <memory>
#include <qcursor.h>
#include <qevent.h>
#include <qfontmetrics.h>
#include <qglobal.h>
#include <qicon.h>
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qstringliteral.h>
#include <qtextcursor.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <qtextobject.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <stacktrace>

namespace LosView
{

    LosEditorUi::LosEditorUi(QWidget *parent)
        : QPlainTextEdit{parent}, LOS_treeSitterDocument(std::make_unique<LosCore::LosTreeSitterDocument>()),
          LOS_treeSitterFoldingProvider(std::make_unique<LosCore::LosTreeSitterFoldingProvider>()),
          LOS_codeFoldingModel(std::make_unique<LosModel::LosCodeFoldingModel>())
    {
        initConnect();
        initStyle();
    }
    LosEditorUi::~LosEditorUi() = default;



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
    void LosEditorUi::showDiagnostic(const QString &file_path, const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &dias)
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
                format.setUnderlineColor(QColor(LosCommon::LosEditorUi_Constants::DIAG_INFO_COLOR));
                break;
            }
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Error:
            {
                format.setUnderlineColor(QColor(LosCommon::LosEditorUi_Constants::DIAG_ERROR_COLOR));
                break;
            }
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Hint:
            {
                format.setUnderlineColor(QColor(LosCommon::LosEditorUi_Constants::DIAG_HINT_COLOR));
                break;
            }
            case LosCommon::LosLsp_Constants::DiagnosticSeverity::Warning:
            {
                format.setUnderlineColor(QColor(LosCommon::LosEditorUi_Constants::DIAG_WARN_COLOR));
                break;
            }
            default:
            {
                format.setUnderlineColor(QColor(LosCommon::LosEditorUi_Constants::DIAG_ERROR_COLOR));
                break;
            }
            }
            // format.setToolTip(a.message);
            selections.format = format;

            QTextDocument *doc    = this->document();
            QTextBlock startBlock = doc->findBlockByNumber(a.startLine);
            int startPos          = startBlock.position() + a.startChar;
            QTextBlock endBlock   = doc->findBlockByNumber(a.endLine);
            int endPos            = endBlock.position() + a.endChar;
            if (!startBlock.isValid() || !endBlock.isValid())
                continue;
            const int docLen = doc->characterCount();
            startPos         = qBound(0, startPos, docLen - 1);
            endPos           = qBound(0, endPos, docLen - 1);
            if (startPos > endPos)
                continue;
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
     * @brief updateSearch
     * 更新搜索 高亮全部匹配并定位到第一个
     *
     * @param text 搜索词
     * @param flags 搜索选项
     * @param regex 是否正则模式
     */
    void LosEditorUi::updateSearch(const QString &text, QTextDocument::FindFlags flags, bool regex)
    {
        L_searchText  = text;
        L_searchFlags = flags;
        L_searchRegex = regex;
        rebuildSearchHighlights();
        if (!L_searchText.isEmpty() && !L_searchMatches.isEmpty())
        {
            setTextCursor(L_searchMatches.first());
            centerCursor();
        }
    }



    /**
     * @brief rebuildSearchHighlights
     * 重新计算匹配并重建高亮（不移动光标）
     */
    void LosEditorUi::rebuildSearchHighlights()
    {
        L_searchMatches = L_searchRegex ? LosCore::LosFinder::findAll(document(), makeSearchExpr())
                                        : LosCore::LosFinder::findAll(document(), L_searchText, L_searchFlags);
        L_searchSelections.clear();
        if (!L_searchText.isEmpty())
        {
            QTextCharFormat format;
            format.setBackground(QColor(LosCommon::LosEditorUi_Constants::SEARCH_HL_BG_COLOR));
            for (const auto &match : L_searchMatches)
            {
                QTextEdit::ExtraSelection selection;
                selection.cursor = match;
                selection.format = format;
                L_searchSelections.append(selection);
            }
        }
        highlightCurrentLine();
    }



    /**
     * @brief 重绘
     */
    void LosEditorUi::rebuildCodeFolding()
    {
        if (!LOS_treeSitterFoldingProvider || !LOS_treeSitterDocument || !LOS_codeFoldingModel)
        {
            return;
        }
        if (!LOS_treeSitterDocument->parse(toPlainText()))
        {
            ERR("解析失败!", "LosEditorUi::rebuildCodeFolding");
            return;
        }
        const QVector<LosCore::LosTreeSitterFoldRange> ranges = LOS_treeSitterFoldingProvider->collect(*LOS_treeSitterDocument);
        LOS_codeFoldingModel->rebuild(ranges);
        applyFoldingVisibility();
    }



    /**
     * @brief makeSearchExpr
     * 根据搜索状态构造正则表达式
     *
     * @return QRegularExpression
     */
    QRegularExpression LosEditorUi::makeSearchExpr() const
    {
        QRegularExpression::PatternOptions options;
        if (!(L_searchFlags & QTextDocument::FindCaseSensitively))
        {
            options |= QRegularExpression::CaseInsensitiveOption;
        }
        QString pattern = L_searchText;
        if (L_searchFlags & QTextDocument::FindWholeWords)
        {
            pattern = QString("\\b(?:%1)\\b").arg(pattern);
        }
        return QRegularExpression(pattern, options);
    }



    /**
     * @brief searchNext 跳转到下一个匹配（循环）
     *
     * @return bool 是否跳转成功
     */
    bool LosEditorUi::searchNext()
    {
        if (L_searchText.isEmpty() || L_searchMatches.isEmpty())
        {
            return false;
        }
        const int current = searchCurrentIndex();
        const int next    = (current % L_searchMatches.size()) + 1;
        setTextCursor(L_searchMatches.at(next - 1));
        centerCursor();
        return true;
    }



    /**
     * @brief searchPrevious 跳转到上一个匹配（循环）
     *
     * @return bool 是否跳转成功
     */
    bool LosEditorUi::searchPrevious()
    {
        if (L_searchText.isEmpty() || L_searchMatches.isEmpty())
        {
            return false;
        }
        const int current = searchCurrentIndex();
        const int prev    = (current + L_searchMatches.size() - 2) % L_searchMatches.size() + 1;
        setTextCursor(L_searchMatches.at(prev - 1));
        centerCursor();
        return true;
    }



    /**
     * @brief clearSearch 清除搜索高亮与状态
     */
    void LosEditorUi::clearSearch()
    {
        L_searchText.clear();
        L_searchFlags = {};
        L_searchRegex = false;
        L_searchMatches.clear();
        L_searchSelections.clear();
        highlightCurrentLine();
    }



    /**
     * @brief replaceCurrent 替换当前选中的匹配，并跳到下一个匹配
     *
     * @param replacement 替换文本
     * @return bool 是否替换成功
     */
    bool LosEditorUi::replaceCurrent(const QString &replacement)
    {
        if (L_searchText.isEmpty() || L_searchMatches.isEmpty())
        {
            return false;
        }
        const int current = searchCurrentIndex();
        if (current <= 0)
        {
            return false;
        }
        const int oldPos = L_searchMatches.at(current - 1).selectionStart();
        QTextCursor edit(L_searchMatches.at(current - 1));
        edit.beginEditBlock();
        edit.insertText(replacement);
        edit.endEditBlock();
        rebuildSearchHighlights();
        if (L_searchMatches.isEmpty())
        {
            return true;
        }
        int nextIndex = 1;
        for (int i = 0; i < L_searchMatches.size(); ++i)
        {
            if (L_searchMatches.at(i).selectionStart() > oldPos)
            {
                nextIndex = i + 1;
                break;
            }
        }
        setTextCursor(L_searchMatches.at(nextIndex - 1));
        centerCursor();
        return true;
    }



    /**
     * @brief replaceAll 从后往前替换全部匹配
     *
     * @param replacement 替换文本
     * @return int 替换数量
     */
    int LosEditorUi::replaceAll(const QString &replacement)
    {
        if (L_searchText.isEmpty() || L_searchMatches.isEmpty())
        {
            return 0;
        }
        const int count = L_searchMatches.size();
        QTextCursor edit(document());
        edit.beginEditBlock();
        for (int i = L_searchMatches.size() - 1; i >= 0; --i)
        {
            const QTextCursor &match = L_searchMatches.at(i);
            edit.setPosition(match.selectionStart());
            edit.setPosition(match.selectionEnd(), QTextCursor::KeepAnchor);
            edit.insertText(replacement);
        }
        edit.endEditBlock();
        rebuildSearchHighlights();
        if (!L_searchMatches.isEmpty())
        {
            setTextCursor(L_searchMatches.first());
            centerCursor();
        }
        return count;
    }



    /**
     * @brief 折叠状态更新
     *
     * @param startLine
     */
    void LosEditorUi::toggleFold(int startLine)
    {
        if (!LOS_codeFoldingModel || !LOS_codeFoldingModel->hasFoldRangeStartingAt(startLine))
        {
            return;
        }
        const bool willCollapse = !LOS_codeFoldingModel->isCollapsed(startLine);
        if (willCollapse)
        {
            const auto &ranges = LOS_codeFoldingModel->foldRanges();
            for (const auto &range : ranges)
            {
                if (range.L_startLine != startLine)
                {
                    continue;
                }
                const int cursorLine = textCursor().blockNumber();
                if (cursorLine > range.L_startLine && cursorLine <= range.L_endLine)
                {
                    const QTextBlock startBlock = document()->findBlockByNumber(startLine);
                    if (!startBlock.isValid())
                    {
                        return;
                    }
                    QTextCursor cursor(document());
                    cursor.setPosition(startBlock.position());
                    setTextCursor(cursor);
                }
                break;
            }
        }
        LOS_codeFoldingModel->toggleFold(startLine);
        applyFoldingVisibility();
    }



    /**
     * @brief searchMatchCount 获取匹配总数
     *
     * @return int
     */
    int LosEditorUi::searchMatchCount() const
    {
        return L_searchMatches.size();
    }



    /**
     * @brief searchCurrentIndex 根据当前光标位置计算当前匹配序号（1 起，无匹配为 0）
     *
     * @return int
     */
    int LosEditorUi::searchCurrentIndex() const
    {
        if (L_searchMatches.isEmpty())
        {
            return 0;
        }
        const int cursorPos = textCursor().position();
        int index           = 0;
        for (int i = 0; i < L_searchMatches.size(); ++i)
        {
            if (L_searchMatches.at(i).selectionEnd() <= cursorPos)
            {
                index = i + 1;
            }
            else
            {
                break;
            }
        }
        return index;
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
        if (LOS_filePath && !LosCore::LosFormatManager::instance().format(&out, LOS_filePath->getFilePath(), toPlainText()))
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



    /**
     * @brief  loadContextAndPath
     *
     * @param context
     * @param file_path
     */
    void LosEditorUi::loadContextAndPath(QSharedPointer<LosModel::LosFileContext> context, QSharedPointer<LosModel::LosFilePath> file_path)
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
        rebuildCodeFolding();
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

        painter.fillRect(event->rect(), QColor(LosCommon::LosEditorUi_Constants::LINENUMBER_BG_COLOR));
        // 当前视口 中 显示的第一个文本块
        QTextBlock block = firstVisibleBlock();

        // blockBoundingGeometry 返回的坐标是相对于 QPlainTextEdit 的可视区域 即视口
        // qRound 四舍五入
        int top    = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
        int bottom = top + qRound(blockBoundingRect(block).height());

        while (block.isValid() && top <= event->rect().bottom())
        {
            const int blockNumber = block.blockNumber();

            if (block.isVisible() && bottom >= event->rect().top())
            {
                const QString number = QString::number(blockNumber + 1);
                if (textCursor().blockNumber() == blockNumber)
                {
                    painter.setPen(QColor(LosCommon::LosEditorUi_Constants::LINENUMBER_CURLINE));
                }
                else
                {
                    painter.setPen(QColor(LosCommon::LosEditorUi_Constants::LINENUMBER_UNCURLINE));
                }
                painter.drawText(0, top,
                                 LOS_lineNumber->width() - LosCommon::LosEditorUi_Constants::FOLD_MARKER_WIDTH -
                                     LosCommon::LosEditorUi_Constants::LINENUMBER_RIGHT_PADDING,
                                 bottom - top, Qt::AlignRight | Qt::AlignVCenter, number);
                const auto foldState = LOS_codeFoldingModel ? LOS_codeFoldingModel->getLineFolderState(blockNumber)
                                                            : LosCommon::LosCodeFoldingModel_Constants::FoldMarkerState::NONE;
                if (foldState != LosCommon::LosCodeFoldingModel_Constants::FoldMarkerState::NONE)
                {
                    const int centerX = LOS_lineNumber->width() - LosCommon::LosEditorUi_Constants::FOLD_MARKER_WIDTH / 2;
                    const int centerY = (top + bottom) / 2;
                    QPolygon marker;
                    if (foldState == LosCommon::LosCodeFoldingModel_Constants::FoldMarkerState::COLLAPSED)
                    {
                        marker << QPoint(centerX - 3, centerY - 5) << QPoint(centerX - 3, centerY + 5) << QPoint(centerX + 4, centerY);
                    }
                    else
                    {
                        marker << QPoint(centerX - 5, centerY - 3) << QPoint(centerX + 5, centerY - 3) << QPoint(centerX, centerY + 4);
                    }

                    painter.save();
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QColor(LosCommon::LosEditorUi_Constants::LINENUMBER_UNCURLINE));
                    painter.drawPolygon(marker);
                    painter.restore();
                }
            }
            block = block.next();
            top   = bottom;
            if (block.isValid())
            {
                bottom = top + qRound(blockBoundingRect(block).height());
            }
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
            emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(LOS_filePath -> getFilePath(), toPlainText());
        }
        return ok;
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
     * @brief getLastSearchText 获取上次搜索词（弹窗关闭后仍保留，用于重新打开时预填）
     *
     * @return QString
     */
    QString LosEditorUi::getLastSearchText() const
    {
        return L_searchText;
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
        int space = LosCommon::LosEditorUi_Constants::FOLD_MARKER_WIDTH + LosCommon::LosLineNumberUi_Constants::BASE_LINEWIDTH +
                    fontMetrics().horizontalAdvance(QLatin1Char('9')) * digit;
        return space;
    }



    /**
     * @brief Get the Block Number By Y 距离
     *
     * @param y
     * @return int
     */
    int LosEditorUi::getBlockNumberByY(int y) const
    {
        auto block = firstVisibleBlock();
        while (block.isValid())
        {
            int top    = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
            int bottom = top + qRound(blockBoundingRect(block).height());
            if (block.isVisible() && y >= top && y < bottom)
            {
                return block.blockNumber();
            }
            if (top > y)
            {
                break;
            }
            block = block.next();
        }
        return -1;
    }



    /**
     * @brief
     *
     * @brief
     *
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
        connect(LOS_completer, QOverload<const QString &>::of(&QCompleter::activated), this, &LosEditorUi::insertCompletion);
        connect(this->document(), &QTextDocument::contentsChange, this, &LosEditorUi::onContentsChange);
        connect(this->document(), &QTextDocument::modificationChanged, this, &LosEditorUi::onModificationChanged);
        connect(L_timer, &QTimer::timeout, this, &LosEditorUi::onDebounceTimeout);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_diagnostics, this, &LosEditorUi::showDiagnostic);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_completion, this, &LosEditorUi::showCompletion);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_hover, this, &LosEditorUi::onHover);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_semanticLegend, this, &LosEditorUi::onSemanticLegend);
        connect(&router, &LosCore::LosRouter::_cmd_lsp_result_semanticTokens, this, &LosEditorUi::onSemanticTokens);
        connect(this, &LosEditorUi::blockCountChanged, this, &LosEditorUi::updateLineNumberAreaWidth);
        connect(this, &LosEditorUi::updateRequest, this, &LosEditorUi::updateLineNumberArea);
        connect(this, &LosEditorUi::cursorPositionChanged, this,
                [this]()
                {
                    LOS_lineNumber->update();
                    updateBrackHighlight();
                    highlightCurrentLine();
                });
        connect(&router, &LosCore::LosRouter::_cmd_openFile_suc, this, &LosEditorUi::onOpenFileSuc);
        connect(&router, &LosCore::LosRouter::_cmd_themeChanged, this,
                [this](const QString &name)
                {
                    if (LOS_highlighter)
                    {
                        LOS_highlighter->setTheme(name);
                    }
                });
    }



    /**
     * @brief initStyle
     * - 设置样式
     * - 设置鼠标 追踪
     */
    void LosEditorUi::initStyle()
    {
        QFontMetrics met(this->font());
        int tab = LosCommon::LosEditorUi_Constants::TAB_WIDTH_SPACES * met.horizontalAdvance(" ");
        this->setTabStopDistance(tab);
        updateLineNumberAreaWidth();
        viewport()->setMouseTracking(true);
        setMouseTracking(true);
    }



    /**
     * @brief 读取 LosCodeFoldingModel 中的折叠状态，并把这个状态真正应用到 QTextDocument 的各个 QTextBlock 上
     *
     */
    void LosEditorUi::applyFoldingVisibility()
    {
        // 保护
        if (!LOS_codeFoldingModel || !document())
        {
            return;
        }

        // 用户点击折叠按钮
        //     ↓
        // LosCodeFoldingModel::toggleFold(3)
        //   修改状态
        //     ↓
        // LosEditorUi::applyFoldingVisibility()
        //     修改 QTextBlock 的可见性

        QTextBlock block = document()->firstBlock();
        while (block.isValid())
        {
            block.setVisible(true);
            block.setLineCount(1);
            // 这里设置 1 的 原因?
            //  setLineCount(1) 表示它重新在文档布局中占据正常的一行
            block = block.next();
        }

        for (const auto &range : LOS_codeFoldingModel->foldRanges())
        {
            // 如果 没有 折叠 就跳过
            if (!LOS_codeFoldingModel->isCollapsed(range.L_startLine))
                continue;
            // 这里 ange.L_startLine + 1 的原因是什么?
            // 2 3 4 5 6
            // 隐藏 3 4 5 6
            // 所以 从 下一个 开始 找
            QTextBlock block = document()->findBlockByNumber(range.L_startLine + 1);
            while (block.isValid() && block.blockNumber() <= range.L_endLine)
            {
                block.setVisible(false);
                // 这里设置 0 的原因是什么?
                // 占据 0
                block.setLineCount(0);
                block = block.next();
            }
        }
        // 主动 设置 脏 然后 全部 更新一下
        document()->markContentsDirty(0, document()->characterCount());
        viewport()->update();
        if (LOS_lineNumber)
        {
            LOS_lineNumber->update();
        }
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
        // 左、上、右、下
        // getLineNumberWidth 获取一个 左侧的长度
        setViewportMargins(getLineNumberWidth(), 0, 0, 0);
    }



    /**
     * @brief highlightCurrentLine 高亮行
     */
    void LosEditorUi::highlightCurrentLine()
    {
        QList<QTextEdit::ExtraSelection> extra;
        extra.append(L_diagnosticSelections);
        if (!isReadOnly())
        {
            QTextEdit::ExtraSelection selection;
            QColor lineColor = QColor(LosCommon::LosEditorUi_Constants::CURLINE_HL_COLOR);
            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extra.append(selection);
        }
        extra.append(L_hoverSelections);
        extra.append(L_bracketSelections);
        extra.append(L_searchSelections);
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
        if (L_lastCursor.selectionStart() == cursor.selectionStart() && L_lastCursor.selectionEnd() == cursor.selectionEnd())
        {
            return;
        }
        L_lastCursor = cursor;
        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format.setFontUnderline(true);
        sel.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        sel.format.setForeground(QColor(LosCommon::LosEditorUi_Constants::HOVER_UNDERLINE_COLOR));
        L_hoverSelections = {sel};
        highlightCurrentLine();
        viewport()->setCursor(Qt::PointingHandCursor);
    }



    /**
     * @brief updateAutoIndent
     *
     * @param event
     */
    bool LosEditorUi::updateAutoIndent(QKeyEvent *event)
    {
        if (event->key() != Qt::Key_Return && event->key() != Qt::Key_Enter)
        {
            return false;
        }
        QTextCursor cur          = textCursor();
        QTextBlock block         = cur.block();
        const QString lineText   = block.text();
        const int col            = cur.positionInBlock();
        const QString textBefore = lineText.left(col);
        const QString textAfter  = lineText.mid(col);

        // 获得前置缩进
        QString baseIndent     = LosCommon::GetLeadingWhiteSpace(textBefore);
        QString nextLineIndent = "";
        if (textBefore.trimmed().endsWith("{"))
        {
            nextLineIndent += LosCommon::LosEditorUi_Constants::BASE_INDENT;
        }
        cur.beginEditBlock();
        bool endBrack   = textAfter.trimmed().endsWith("}");
        bool beginBrack = textBefore.trimmed().startsWith("{");
        if (beginBrack && endBrack)
        {
            cur.insertText("\n" + baseIndent + nextLineIndent + "\n" + baseIndent);
            cur.movePosition(QTextCursor::Up);
            cur.movePosition(QTextCursor::EndOfLine);
        }
        else
        {
            cur.insertText("\n" + baseIndent + nextLineIndent);
        }
        cur.endEditBlock();
        setTextCursor(cur);
        return true;
    }



    /**
     * @brief clearHoverUnderline 清理下划线
     */
    void LosEditorUi::clearHoverUnderline()
    {
        L_hoverSelections.clear();
        L_lastCursor = QTextCursor();
        highlightCurrentLine();
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
        html.replace("&", "&amp;");
        html.replace("<", "&lt;");
        html.replace(">", "&gt;");
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_CPP, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_OPEN);
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_C, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_OPEN);
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_PLAIN, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_CLOSE);
        QRegularExpression boldRegex(LosCommon::LosEditorUi_Constants::HOVER_BOLD_REGEX);
        html.replace(boldRegex, LosCommon::LosEditorUi_Constants::HOVER_BOLD_REPLACE);
        QRegularExpression inlineCodeRegex(LosCommon::LosEditorUi_Constants::HOVER_INLINE_CODE_REGEX);
        html.replace(inlineCodeRegex, LosCommon::LosEditorUi_Constants::HOVER_INLINE_CODE_REPLACE);
        html.replace("\n", LosCommon::LosEditorUi_Constants::HOVER_LINE_BREAK);
        showHoverPopup(html);
    }



    /**
     * @brief onHover_Rust
     * - 处理 rust-analyzer 返回的 hover Markdown
     * - 与 Clangd 版本基本一致, 区别在代码块语言标记是 ```rust
     *
     * @param markdownContent
     */
    void LosEditorUi::onHover_Rust(const QString &markdownContent)
    {
        if (markdownContent.isEmpty())
        {
            hideHoverPopup();
            return;
        }
        QString html = markdownContent;
        html.replace("&", "&amp;");
        html.replace("<", "&lt;");
        html.replace(">", "&gt;");
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_RUST, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_OPEN);
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_PLAIN, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_CLOSE);
        QRegularExpression boldRegex(LosCommon::LosEditorUi_Constants::HOVER_BOLD_REGEX);
        html.replace(boldRegex, LosCommon::LosEditorUi_Constants::HOVER_BOLD_REPLACE);
        QRegularExpression inlineCodeRegex(LosCommon::LosEditorUi_Constants::HOVER_INLINE_CODE_REGEX);
        html.replace(inlineCodeRegex, LosCommon::LosEditorUi_Constants::HOVER_INLINE_CODE_REPLACE);
        html.replace("\n", LosCommon::LosEditorUi_Constants::HOVER_LINE_BREAK);
        showHoverPopup(html);
    }



    /**
     * @brief onHover_CMake
     * - 处理 neocmakelsp 返回的 hover Markdown
     * - 与 Clangd/Rust 版本基本一致, 区别在代码块语言标记是 ```cmake
     *
     * @param markdownContent
     */
    void LosEditorUi::onHover_CMake(const QString &markdownContent)
    {
        if (markdownContent.isEmpty())
        {
            hideHoverPopup();
            return;
        }
        QString html = markdownContent;
        html.replace("&", "&amp;");
        html.replace("<", "&lt;");
        html.replace(">", "&gt;");
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_CMAKE, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_OPEN);
        html.replace(LosCommon::LosEditorUi_Constants::MD_FENCE_PLAIN, LosCommon::LosEditorUi_Constants::HOVER_CODE_BLOCK_CLOSE);
        QRegularExpression boldRegex(LosCommon::LosEditorUi_Constants::HOVER_BOLD_REGEX);
        html.replace(boldRegex, LosCommon::LosEditorUi_Constants::HOVER_BOLD_REPLACE);
        QRegularExpression inlineCodeRegex(LosCommon::LosEditorUi_Constants::HOVER_INLINE_CODE_REGEX);
        html.replace(inlineCodeRegex, LosCommon::LosEditorUi_Constants::HOVER_INLINE_CODE_REPLACE);
        html.replace("\n", LosCommon::LosEditorUi_Constants::HOVER_LINE_BREAK);
        showHoverPopup(html);
    }



    /**
     * @brief hideCompletionPopup
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

        int idealWidth = LOS_completer->popup()->sizeHintForColumn(0);
        r.setWidth(qMin(idealWidth + LosCommon::LosEditorUi_Constants::COMPLETION_POPUP_PADDING,
                        LosCommon::LosEditorUi_Constants::COMPLETION_POPUP_MAX_WIDTH));

        LOS_completer->complete(r);
        L_showComplete = true;
        return true;
    }



    /**
     * @brief onContentsChange
     * - 带参数版的内容变化信号 (QTextDocument::contentsChange)
     * - rehighlight() 只改字符格式不改内容, 此时 charsRemoved==charsAdded==0
     *   据此过滤掉高亮重绘伪造的变化, 从根上断开
     *   contentsChange -> semantic -> rehighlight -> contentsChange 死循环
     */
    void LosEditorUi::onContentsChange(int from, int charsRemoved, int charsAdded)
    {
        Q_UNUSED(from);
        if (charsRemoved == 0 && charsAdded == 0)
            return;
        if (!LOS_context)
            return;
        L_timer->start(LosCommon::LosEditorUi_Constants::DEBOUNCE_INTERVAL_MS);
    }



    /**
     * @brief onModificationChanged 文件改动
     *
     * @param changed
     */
    void LosEditorUi::onModificationChanged(bool changed)
    {
        if (!LOS_filePath)
            return;
        L_dirty = changed;
        emit LosCore::LosRouter::instance()._cmd_fileDirty(LOS_filePath -> getFilePath(), changed);
    }



    /**
     * @brief onDebounceTimeout
     * 防抖语法补全
     * 输入修改之后 开启的时间结束 触发的
     */
    void LosEditorUi::onDebounceTimeout()
    {
        if (!LOS_filePath)
            return;
        rebuildCodeFolding();
        const QString filePath = LOS_filePath->getFilePath();
        emit LosCore::LosRouter::instance()._cmd_lsp_request_textChanged(filePath, this -> toPlainText());
        emit LosCore::LosRouter::instance()._cmd_lsp_request_semantic(filePath);
        QTextCursor cursor = this->textCursor();
        int line           = cursor.blockNumber();
        int col            = cursor.positionInBlock();
        if (col == 0)
            return;
        QString currentLineText = cursor.block().text();
        QChar lastChar          = currentLineText.at(col - 1);
        bool isTriggerChar      = lastChar.isLetterOrNumber() || lastChar == '_' || lastChar == '.' || lastChar == '>' || lastChar == ':' ||
                             lastChar == '#' || lastChar == '/' || lastChar == '"' || lastChar == '<';
        if (!isTriggerChar)
            return;
        if (lastChar == '>' && (col < 2 || currentLineText.at(col - 2) != '-'))
            return;
        if (lastChar == ':' && (col < 2 || currentLineText.at(col - 2) != ':'))
            return;
        L_oldWord = getWordUnderCursor();
        emit LosCore::LosRouter::instance()._cmd_lsp_request_completeion(LOS_filePath -> getFilePath(), line, col);
    }



    /**
     * @brief onHover
     *
     * @param markdownContent
     */
    void LosEditorUi::onHover(const QString &absolute_file_path, const QString &markdownContent)
    {
        if (!LOS_filePath || LOS_filePath->getAbsoluteFilePath() != absolute_file_path)
            return;
        auto lang = LosCommon::CheckLang(absolute_file_path);
        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            onHover_Clangd(markdownContent);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
        {
            onHover_Rust(markdownContent);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::CMAKE:
        {
            onHover_CMake(markdownContent);
            break;
        }
        default:
            break;
        }
    }



    /**
     * @brief showHoverPopup 显示自绘 hover 浮窗
     * @param html 已转换为 rich text 的 hover 内容
     */
    void LosEditorUi::showHoverPopup(const QString &html)
    {
        QPoint anchor = L_lastHoverWordRectGlobal.isValid()
                            ? L_lastHoverWordRectGlobal.bottomLeft()
                            : L_lastHoverGlobal + QPoint(0, LosCommon::LosEditorUi_Constants::HOVER_ANCHOR_FALLBACK_Y);
        LosHoverPopup::instance().showPopup(html, anchor, L_lastHoverWordRectGlobal);
    }



    /**
     * @brief hideHoverPopup 隐藏自绘 hover 浮窗
     */
    void LosEditorUi::hideHoverPopup()
    {
        LosHoverPopup::instance().hidePopup();
        L_lastHoverWord.clear();
    }



    /**
     * @brief updateBrackHighlight 高亮括号
     */
    void LosEditorUi::updateBrackHighlight()
    {
        L_bracketSelections.clear();
        if (document()->isEmpty())
        {
            highlightCurrentLine();
            return;
        }
        QTextCursor cursor = textCursor();
        int pos            = cursor.position();
        int brackPos       = -1;
        QChar bracket;
        if (pos > 0)
        {
            QChar prevChar = document()->characterAt(pos - 1);
            if (LosCommon::IsLeftBrack(prevChar) || LosCommon::IsRightBrack(prevChar))
            {
                brackPos = pos - 1;
                bracket  = prevChar;
            }
        }
        if (brackPos == -1 && pos < document()->characterCount())
        {
            QChar curChar = document()->characterAt(pos);
            if (LosCommon::IsLeftBrack(curChar) || LosCommon::IsRightBrack(curChar))
            {
                brackPos = pos;
                bracket  = curChar;
            }
        }
        int direction = LosCommon::IsLeftBrack(bracket) ? 1 : -1;
        int matchPos  = LosCommon::FindMatchingBracket(*document(), brackPos, bracket, direction);
        if (matchPos == -1)
        {
            return;
        }
        QTextCharFormat format;
        format.setBackground(QColor(LosCommon::LosEditorUi_Constants::BRACKET_MATCH_BG_COLOR));
        format.setForeground(Qt::black);
        format.setFontWeight(QFont::Bold);
        QTextEdit::ExtraSelection sel1;
        QTextCursor c1(document());
        c1.setPosition(brackPos);
        c1.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        sel1.cursor = c1;
        sel1.format = format;
        QTextEdit::ExtraSelection sel2;
        QTextCursor c2(document());
        c2.setPosition(matchPos);
        c2.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        sel2.cursor = c2;
        sel2.format = format;
        L_bracketSelections.append(sel1);
        L_bracketSelections.append(sel2);
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
     * @param absolute_file_path token 所属文件
     * @param data 语义 token 数组
     *
     * 必须按文件路径过滤: 该信号是全局广播, 同时打开多个文件时, 每个编辑器的
     * highlighter 都会收到所有文件的 token. 不过滤会导致 A 文件的 token 被
     * B 文件的 highlighter 套用 -> 高亮整体错位/张冠李戴.
     */
    void LosEditorUi::onSemanticTokens(const QString &absolute_file_path, const QJsonArray &data)
    {
        if (!LOS_filePath || LOS_filePath->getAbsoluteFilePath() != absolute_file_path)
            return;
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



    /**
     * @brief
     *
     * 光标拦截
     * 弹出 语法补全
     * 同时 支持 括号补全
     * @param event
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

        if (updateAutoIndent(event))
        {
            return;
        }

        if (event->matches(QKeySequence::Cut))
            onCutKeyClicked();
        if (event->matches(QKeySequence::Copy))
            onCopyCurrentLine();

        static const QHash<QChar, QChar> AUTO_CLOSE_MAP{{'{', '}'}, {'[', ']'}, {'(', ')'}};
        if (!event->text().isEmpty())
        {
            const QChar inputChar = event->text().at(0);

            /*
             * 输入右括号时
             * 如果光标右侧已经是相同的右括号，则直接跳过
             * 避免把 () 输入成 ())
             * 有选区时不跳过，保留 Qt 默认的“输入替换选区”行为
             */
            const QChar openingChar = AUTO_CLOSE_MAP.key(inputChar, QChar());

            if (!openingChar.isNull())
            {
                QTextCursor cursor = textCursor();
                if (!cursor.hasSelection() && document()->characterAt(cursor.position()) == inputChar)
                {
                    cursor.movePosition(QTextCursor::NextCharacter);
                    setTextCursor(cursor);
                    return;
                }
            }

            /*
             * 输入左括号时 自动补全对应的右括号
             * 光标位于两个括号之间
             * beginEditBlock/endEditBlock 使左右括号作为一次
             * 撤销操作处理
             */
            if (AUTO_CLOSE_MAP.contains(inputChar))
            {
                QTextCursor cursor = textCursor();

                cursor.beginEditBlock();

                cursor.insertText(QString(inputChar));
                cursor.insertText(QString(AUTO_CLOSE_MAP.value(inputChar)));

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
     LosLineNumberUi::mousePressEvent()
        ↓
    判断是否点击折叠标记区域
        ↓
    LosEditorUi::toggleFold(line)
        ↓
    LosCodeFoldingModel::toggleFold(line)
        ↓
    applyFoldingVisibility()

     * @param event
     */
    void LosEditorUi::mousePressEvent(QMouseEvent *event)
    {

        if (!LOS_filePath)
            return;
        hideCompletionPopup();
        hideHoverPopup();
        // ctrl + 左键 的 效果
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
            // 点击了 ctrl
            //
            updateHoverUnderline(event->pos());
        }
        // 鼠标离开当前悬停的单词矩形则隐藏 hover 浮窗
        if (LosHoverPopup::instance().isVisible() && L_lastHoverWordRectGlobal.isValid())
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
            int tab = LosCommon::LosEditorUi_Constants::TAB_WIDTH_SPACES * met.horizontalAdvance(" ");
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

            if (word == L_lastHoverWord && LosHoverPopup::instance().isVisible())
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