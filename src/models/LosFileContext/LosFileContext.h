

#pragma once

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>

namespace LosModel {

class LosFileContext {

public:
  bool load(const QString &file_path);
  bool save(const QString &file_path);
  bool save();
  QString getContent() const;
  void setContent(const QString &content);
  bool isDirty() const;
  void setDirty(bool dirty);
  bool isLoaded() const;
  [[nodiscard]] static LosFileContext* create();

public:
 
private:
  QString L_content = "";
  QString L_curFilePath = "";
  bool L_isDirty = false;
  bool L_isLoaded = false;
};
}; // namespace LosModel