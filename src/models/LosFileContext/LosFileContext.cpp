

#include "LosFileContext.h"

namespace LosModel {
LosFileContext* LosFileContext::create() {
  LosFileContext* text = new LosFileContext();
  return text;
}

bool LosFileContext::load(const QString &file_path) {
  QFile file(file_path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }
  QTextStream in(&file);
  in.setEncoding(QStringConverter::Utf8);
  L_content = in.readAll();
  file.close();
  L_curFilePath = file_path;
  L_isDirty = false;
  L_isLoaded = true;
  return true;
}

bool LosFileContext::save(const QString &file_path) {
  if (file_path.isEmpty())
    return false;
  QFile file(file_path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return false;
  }
  QTextStream out(&file);
  out.setEncoding(QStringConverter::Utf8);
  out << L_content;
  file.close();
  L_curFilePath = file_path;
  L_isDirty = false;
  return true;
}

bool LosFileContext::save() { return save(L_curFilePath); }
QString LosFileContext::getContent() const { return L_content; }
void LosFileContext::setContent(const QString &content) { L_content = content; }
bool LosFileContext::isDirty() const { return L_isDirty; }
bool LosFileContext::isLoaded() const { return L_isLoaded; }
void LosFileContext::setDirty(bool dirty) { L_isDirty = dirty; }

}; // namespace LosModel