

#pragma once
#include "common/constants/ConstantsClass.h"
#include "core/LosHighlighter/LosHighlighter.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosCompleterUi/LosCompleterUi.h"

#include <QApplication>
#include <QObject>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>
#include <qdebug.h>
#include <qevent.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qplaintextedit.h>
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

  private: // construct
    explicit LosEditorUi(QWidget *parent = nullptr);

  public: // tool
    static LosEditorUi *create(QWidget *parent = nullptr);
    void showCompletion(const QStringList &list);
    void showDiagnostic(const QString &file_path, const QList<LosCommon::LosDiagnostic> &);
    void gotoLine(int line);

  public: // get
    bool isDirty() const;
    QString getWordUnderCursor() const;

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

  protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

  signals:
    private: // param
    QTimer *L_timer                       = nullptr;
    LosModel::LosFileContext *LOS_context = nullptr;
    LosModel::LosFilePath *LOS_filePath   = nullptr;
    LosView::LosCompleterUi *LOS_completer;
    LosCore::LosHighlighter *LOS_highlighter = nullptr;

  signals:
    void _editorDirty(bool is_dirty);
};
} // namespace LosView