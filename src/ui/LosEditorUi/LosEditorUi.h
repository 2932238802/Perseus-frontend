

#pragma once
#include <QObject>
#include <qdebug.h>
#include <qevent.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qplaintextedit.h>
namespace LosModel {
class LosFileContext;
}

namespace LosView {

class LosEditorUi : public QPlainTextEdit {
  Q_OBJECT

signals:
  void _dirtyChanged(const QString &file_path, bool is_dirty);

public:
  ~LosEditorUi() override;

private: // construct
  explicit LosEditorUi(QWidget *parent = nullptr);

public: // tool
  static LosEditorUi *create(QWidget *parent = nullptr);

public: // get
  QString getCurFilePath();
  bool isDirty() const;

public: // set
  void loadContext(LosModel::LosFileContext *context);
  bool save();

private: // init
  void initConnect();

private slots: // chs
  void onTextChanged();

private: // param
  LosModel::LosFileContext *LOS_context = nullptr;
  bool L_isDirty = false;

signals:
  void _editorDirty(bool is_dirty);
};
} // namespace LosView