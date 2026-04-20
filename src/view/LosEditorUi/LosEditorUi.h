#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsNum.h"
#include "core/LosFormat/LosFormatManager/LosFormatManager.h"
#include "core/LosHighlighter/LosHighlighter.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosCompleterUi/LosCompleterUi.h"

#include <QApplication>
#include <QCompleter>
#include <QDebug>
#include <QEvent>
#include <QFileInfo>
#include <QObject>
#include <QPainter>
#include <QScrollBar>
#include <QSharedPointer>
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>
#include <QToolTip>
#include <QWidget>
#include <algorithm>
#include <atomic>
#include <qcoreevent.h>
#include <qevent.h>
#include <qfontmetrics.h>
#include <qglobal.h>
#include <qicon.h>
#include <qkeysequence.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qplaintextedit.h>
#include <qtextcursor.h>
#include <qtextdocument.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <qtextobject.h>
#include <qtooltip.h>
#include <qvariant.h>

namespace LosModel
{
    class LosFileContext;
}

namespace LosView
{
    class LosLineNumberUi;
    class LosEditorUi : public QPlainTextEdit
    {
        Q_OBJECT
      public: /* construct */
        ~LosEditorUi() override;
        explicit LosEditorUi(QWidget *parent = nullptr);

      public: /* tool */
        void showCompletion(const QStringList &list);
        void showDiagnostic(const QString &file_path, const QList<LosCommon::LosLsp_Constants::LosDiagnostic> &);
        void gotoLine(int line);
        void format();

      public: /* get */
        QString getWordUnderCursor() const;
        bool isDirty() const;
        int getLineNumberWidth() const;

      public: /* set */
        void loadContextAndPath(QSharedPointer<LosModel::LosFileContext> context,
                                QSharedPointer<LosModel::LosFilePath> file_path);
        bool save();
        void insertCompletion(const QString &completion);
        void lineNumberAreaPaintEvent(QPaintEvent *event);

      private: /* init */
        void initConnect();
        void initStyle();

      private: /* tool */
        void updateLineNumberArea(const QRect &rect, int dy);
        void updateLineNumberAreaWidth();
        void highlightCurrentLine();
        void updateHoverUnderline(const QPoint &vpPos);
        void clearHoverUnderline();

      private slots: /* chs */
        void onTextChanged();
        void onDebounceTimeout();
        void onHover_Clangd(const QString &markdownContent);
        void onSemanticLegend(const QStringList &, const QStringList &);
        void onSemanticTokens(const QJsonArray &);
        void onCutKeyClicked();
        void onCopyCurrentLine();
        void onControlKeyPressed();

      protected: /* override */
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void changeEvent(QEvent *e) override;
        bool event(QEvent *event) override;
        void resizeEvent(QResizeEvent *e) override;
        void focusOutEvent(QFocusEvent *event) override;

      private: /* param */
        std::atomic<bool> L_showComplete                        = false;
        bool L_dirty                                            = false;
        bool L_ctrlBtnPresses                                   = false;
        QTextCursor L_lastCursor                                = QTextCursor{};
        QString L_oldWord                                       = "";
        QTimer *L_timer                                         = nullptr;
        QPoint L_lastHoverGlobal                                = QPoint();
        QSharedPointer<LosModel::LosFileContext> LOS_context    = {};
        QSharedPointer<LosModel::LosFilePath> LOS_filePath      = {};
        QList<QTextEdit::ExtraSelection> L_diagnosticSelections = {};
        QList<QTextEdit::ExtraSelection> L_hoverSelections      = {};
        LosView::LosCompleterUi *LOS_completer                  = nullptr;
        LosCore::LosHighlighter *LOS_highlighter                = nullptr;
        LosView::LosLineNumberUi *LOS_lineNumber                = nullptr;
    };
} /* namespace LosView */