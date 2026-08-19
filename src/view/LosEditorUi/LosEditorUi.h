// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "common/constants/ConstantsClass/LosLspClass.h"

#include <QEvent>
#include <QJsonArray>
#include <QRegularExpression>
#include <QSharedPointer>
#include <QTextCursor>
#include <QTextDocument>
#include <QTimer>
#include <QWidget>
#include <atomic>
#include <memory>
#include <qplaintextedit.h>

namespace LosModel
{
    class LosFileContext;
    class LosFilePath;
    class LosCodeFoldingModel;
} // namespace LosModel

namespace LosCore
{
    class LosHighlighter;
    class LosTreeSitterDocument;
    class LosTreeSitterFoldingProvider;
} // namespace LosCore

namespace LosView
{
    class LosCompleterUi;
    class LosLineNumberUi;

    /**
     * @brief LosEditorUi
     */
    class LosEditorUi : public QPlainTextEdit
    {
        Q_OBJECT
      public: // construct
        ~LosEditorUi() override;
        explicit LosEditorUi(QWidget *parent = nullptr);

      public: // tool
        void showCompletion(const QStringList &list);
        void showDiagnostic(const QString &file_path, const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &);
        void gotoLine(int line);
        void format();
        void updateSearch(const QString &text, QTextDocument::FindFlags flags = QTextDocument::FindFlags(), bool regex = false);
        bool searchNext();
        bool searchPrevious();
        bool replaceCurrent(const QString &replacement);
        int replaceAll(const QString &replacement);
        void clearSearch();
        int searchMatchCount() const;
        int searchCurrentIndex() const;
        void toggleFold(int startLine);

      public: // get
        bool isDirty() const;
        QString getLastSearchText() const;
        QString getWordUnderCursor() const;
        int getLineNumberWidth() const;
        int getBlockNumberByY(int y) const;

      public: // set
        void loadContextAndPath(QSharedPointer<LosModel::LosFileContext> context, QSharedPointer<LosModel::LosFilePath> file_path);
        bool save();
        void insertCompletion(const QString &completion);
        void lineNumberAreaPaintEvent(QPaintEvent *event);

      private: // init
        void initConnect();
        void initStyle();

      private:                         // tool
        void applyFoldingVisibility(); // 读取 LosCodeFoldingModel 中的折叠状态，并把这个状态真正应用到 QTextDocument 的各个 QTextBlock 上
        void updateLineNumberArea(const QRect &rect, int dy);
        void updateLineNumberAreaWidth();
        void updateBrackHighlight();
        void updateHoverUnderline(const QPoint &vpPos);
        bool updateAutoIndent(QKeyEvent *event);
        void highlightCurrentLine();
        void rebuildCodeFolding();
        void rebuildSearchHighlights();
        QRegularExpression makeSearchExpr() const;
        bool repositionCompletionPopup();
        void showHoverPopup(const QString &html);
        void hideHoverPopup();
        void hideCompletionPopup();
        void clearHoverUnderline();

      private: // onhover
        void onHover_Clangd(const QString &markdownContent);
        void onHover_Rust(const QString &markdownContent);
        void onHover_CMake(const QString &markdownContent);

      private slots: // chs
        void onContentsChange(int from, int charsRemoved, int charsAdded);
        void onModificationChanged(bool changed);
        void onDebounceTimeout();
        void onHover(const QString &absolute_file_path, const QString &markdownContent);
        void onSemanticLegend(const QStringList &, const QStringList &);
        void onSemanticTokens(const QString &absolute_file_path, const QJsonArray &);
        void onCutKeyClicked();
        void onCopyCurrentLine();
        void onControlKeyPressed();
        void onOpenFileSuc(const QString &filepath);

      protected: // override
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void changeEvent(QEvent *e) override;
        bool event(QEvent *event) override;
        void resizeEvent(QResizeEvent *e) override;
        void focusOutEvent(QFocusEvent *event) override;

      private: // param
        std::atomic<bool> L_showComplete                        = false;
        bool L_dirty                                            = false;
        bool L_ctrlBtnPresses                                   = false;
        QTextCursor L_lastCursor                                = QTextCursor{};
        QString L_oldWord                                       = "";
        QTimer *L_timer                                         = nullptr;
        QPoint L_lastHoverGlobal                                = QPoint();
        QRect L_lastHoverWordRectGlobal                         = QRect();
        QString L_lastHoverWord                                 = "";
        QSharedPointer<LosModel::LosFileContext> LOS_context    = {};
        QSharedPointer<LosModel::LosFilePath> LOS_filePath      = {};
        QList<QTextEdit::ExtraSelection> L_bracketSelections    = {};
        QList<QTextEdit::ExtraSelection> L_diagnosticSelections = {};
        QList<QTextEdit::ExtraSelection> L_hoverSelections      = {};
        QList<QTextEdit::ExtraSelection> L_searchSelections     = {};
        QString L_searchText                                    = "";
        QTextDocument::FindFlags L_searchFlags                  = {};
        bool L_searchRegex                                      = false;
        QList<QTextCursor> L_searchMatches                      = {};      // 搜索匹配
        LosView::LosCompleterUi *LOS_completer                  = nullptr; // 语法 补全
        LosCore::LosHighlighter *LOS_highlighter                = nullptr; // 语法 高亮
        LosView::LosLineNumberUi *LOS_lineNumber                = nullptr; // 数字号
        std::unique_ptr<LosCore::LosTreeSitterDocument> LOS_treeSitterDocument;
        std::unique_ptr<LosCore::LosTreeSitterFoldingProvider> LOS_treeSitterFoldingProvider;
        std::unique_ptr<LosModel::LosCodeFoldingModel> LOS_codeFoldingModel;
    };
} /* namespace LosView */