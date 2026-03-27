

#pragma once
#include "common/constants/ConstantsClass.h"
#include "core/LosFormatManager/LosFormatManager.h"
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
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>
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
    void loadContextAndPath(LosModel::LosFileContext *context, LosModel::LosFilePath *file_path);
    bool save();
    void insertCompletion(const QString &completion);

  private: // init
    void initConnect();
    void initStyle();

  private slots: // chs
    void onTextChanged();
    void onDebounceTimeout();

  protected: // override
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void changeEvent(QEvent *e) override;

  private: // param
    bool L_dirty                          = false;
    QString L_oldWord                     = "";
    QTimer *L_timer                       = nullptr;
    LosModel::LosFileContext *LOS_context = nullptr;
    LosModel::LosFilePath *LOS_filePath   = nullptr;
    LosView::LosCompleterUi *LOS_completer;
    LosCore::LosHighlighter *LOS_highlighter = nullptr;
};
} // namespace LosView