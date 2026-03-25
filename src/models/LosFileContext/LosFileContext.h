

#pragma once

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>

namespace LosModel
{

class LosFileContext
{
  public:
    QString load(const QString &file_path);
    bool save(const QString &content, const QString &file_path);
    bool isDirty() const;
    void setDirty(bool dirty);
    bool isLoaded() const;
    [[nodiscard]] static LosFileContext *create();

  public:
  private:
    QString L_content = "";
    bool L_isDirty    = false;
    bool L_isLoaded   = false;
};
}; // namespace LosModel