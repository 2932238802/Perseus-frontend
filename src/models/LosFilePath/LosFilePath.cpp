
#include "LosFilePath.h"
#include <qdebug.h>
#include <qfileinfo.h>

namespace LosModel {
/**
构造和析构函数
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
    qWarning() << "LosFilePath::loadFile: unrecognized file type -> "
               << file_path;
  }
}

const QString &LosFilePath::getFilePath() const { return L_filePath; }
const QString &LosFilePath::getFileName() const { return L_fileName; }
QString LosFilePath::getBaseFileName() const {
  return QFileInfo(L_filePath).baseName();
};
QString LosFilePath::getAbsolutePath() const {
  return QFileInfo(L_filePath).absolutePath();
};
QString LosFilePath::getAbsoluteFilePath() const {
  return QFileInfo(L_filePath).absoluteFilePath();
}
LosCommon::LOS_ENUM_FileType LosFilePath::getFileType() const {
  return LOS_fileType;
}
bool LosFilePath::isDir() const { return QFileInfo(L_filePath).isDir(); }
bool LosFilePath::isFile() const { return QFileInfo(L_filePath).isFile(); }
bool LosFilePath::isExist() const { return QFileInfo(L_filePath).exists(); }

QDebug operator<<(QDebug debug, const LosFilePath &path) {
  QDebugStateSaver saver(debug);
  debug.nospace();
  debug << "LosFilePath(" << "Name: \"" << path.getFileName() << "\", "
        << "Path: \"" << path.getFilePath() << "\", "
        << "Type: " << (path.isDir() ? "Folder" : "File") << ", "
        << "Exists: " << (path.isExist() ? "True" : "False") << ")";
  return debug;
}
QStringList &operator<<(QStringList &list, const LosFilePath &path) {
  list.append(path.getFilePath());
  return list;
}

/**
对比
*/
bool LosFilePath::operator==(const QString &str) const {
  return L_filePath == str;
}

} // namespace LosModel