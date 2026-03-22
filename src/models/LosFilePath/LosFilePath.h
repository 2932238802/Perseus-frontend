#pragma once
#include "common/enum/LosFileType.h"
#include <QString>
#include <qobject.h>
#include <qtmetamacros.h>

namespace LosModel {
class LosFilePath  {
public:
  LosFilePath();
  explicit LosFilePath(const QString&);

public: // init
  void loadFile(const QString &file_path);

public: // get
  const QString& getFilePath() const;
  const QString& getFileName() const;
  LosCommon::LOS_ENUM_FileType getFileType() const;
  bool isDir() const;
  bool isFile() const;
  bool isExist() const;

private:
  QString L_filePath;
  QString L_fileName;
  LosCommon::LOS_ENUM_FileType LOS_fileType;
};
}; // namespace LosModel