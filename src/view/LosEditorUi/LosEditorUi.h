

#pragma once
#include "common/constants/ConstantsClass.h"
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
#include <QScrollBar>
#include <QSharedPointer>
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>
#include <QToolTip>
#include <QWidget>
#include <atomic>
#include <qcoreevent.h>
#include <qfontmetrics.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <qplaintextedit.h>
#include <qtextcursor.h>
#include <qtextdocument.h>
#include <qtextedit.h>
#include <qtextformat.h>
#include <qtextobject.h>
#include <qvariant.h>

namespace LosModel
{
class LosFileContext;
}

namespace LosView
{

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

  public: // get
    QString getWordUnderCursor() const;
    bool isDirty() const;

  public: // set
    void loadContextAndPath(QSharedPointer<LosModel::LosFileContext> context,
                            QSharedPointer<LosModel::LosFilePath> file_path);
    bool save();
    void insertCompletion(const QString &completion);

  private: // init
    void initConnect();
    void initStyle();

  private: // tool
    void cutCurrentLine();
    void copyCurrentLine();

  private slots: // chs
    void onTextChanged();
    void onDebounceTimeout();
    void onHover_Clangd(const QString &markdownContent);
    void onSemanticLegend(const QStringList &, const QStringList &);
    void onSemanticTokens(const QJsonArray &);

  protected: // override
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void changeEvent(QEvent *e) override;
    bool event(QEvent *event) override;

  private: // param
    std::atomic<bool> L_showComplete = false;
    bool L_dirty                     = false;

    QString L_oldWord                = "";
    QTimer *L_timer                  = nullptr;
    QPoint L_lastHoverGlobal         = QPoint();
    QSharedPointer<LosModel::LosFileContext> LOS_context;
    QSharedPointer<LosModel::LosFilePath> LOS_filePath;
    LosView::LosCompleterUi *LOS_completer;
    LosCore::LosHighlighter *LOS_highlighter = nullptr;
};
} // namespace LosView