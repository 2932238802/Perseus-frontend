
#include "LosFilePath.h"
#include <qfileinfo.h>

namespace LosModel {
/**
 */
LosFilePath::LosFilePath() : L_fileName(""), L_filePath("") {}

LosFilePath::LosFilePath(const QString &file_path) { loadFile(file_path); }

/**
加载文件
*/
void LosFilePath::loadFile(const QString &file_path) {
  L_filePath = file_path;
  QFileInfo fileInfo(file_path);
  L_fileName = QFileInfo(file_path).fileName();

  if (fileInfo.isDir()) {
    LOS_fileType = LosCommon::LOS_ENUM_FileType::FT_FOLDER;
  } else if (fileInfo.isFile()) {
    LOS_fileType = LosCommon::GetFileType(fileInfo.suffix());
  } else {
    qWarning() << "LosFilePath::loadFile: 无法识别的文件类型 -> " << file_path;
  }
}

const QString& LosFilePath::getFilePath() const { return L_filePath; }
const QString& LosFilePath::getFileName() const { return L_fileName; }
LosCommon::LOS_ENUM_FileType LosFilePath::getFileType() const {
  return LOS_fileType;
}
bool LosFilePath::isDir() const { return QFileInfo(L_filePath).isDir(); }
bool LosFilePath::isFile() const { return QFileInfo(L_filePath).isFile(); }
bool LosFilePath::isExist() const { return QFileInfo(L_filePath).exists(); }
} // namespace LosModel