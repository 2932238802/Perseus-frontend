

#pragma once
#include "common/constants/ConstantsClass.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosCompleterUi/LosCompleterUi.h"
#include <QObject>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>
#include <qdebug.h>
#include <qevent.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qplaintextedit.h>
#include <qtimer.h>
namespace LosModel {
class LosFileContext;
}

namespace LosView {

class LosEditorUi : public QPlainTextEdit {
  Q_OBJECT

public: // construct
  ~LosEditorUi() override;

private: // construct
  explicit LosEditorUi(QWidget *parent = nullptr);

public: // tool
  static LosEditorUi *create(QWidget *parent = nullptr);
  void showCompletion(const QStringList &list);
  void showDiagnostic(const QString& file_path,const QList<LosCommon::LosDiagnostic>&);

public: // get
  bool isDirty() const;
  QString getWordUnderCursor() const;

public: // set
  void loadContextAndPath(LosModel::LosFileContext *context,
                          LosModel::LosFilePath *file_path);
  bool save();
  void insertCompletion(const QString &completion);

private: // init
  void initConnect();
  void initStyle();

private slots: // chs
  void onTextChanged();

protected:
  void keyPressEvent(QKeyEvent *event) override;

signals:
  void _completionRequest(const QString &file_path, int line, int col);
  void _textChangedForLsp(const QString &file_path,
                          const QString &file_content);
  void _openFileForLsp(const QString &file_path, const QString &file_content);

private: // param
  LosModel::LosFileContext *LOS_context = nullptr;
  LosModel::LosFilePath *LOS_filePath = nullptr;
  QTimer *L_timer = nullptr;
  LosView::LosCompleterUi *LOS_completer;

signals:
  void _editorDirty(bool is_dirty);
};
} // namespace LosView