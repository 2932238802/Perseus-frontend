#pragma once
#include "common/enum/LosFileType.h"
#include "core/LosLog/LosLog.h"

#include <QDebug>
#include <QDir>
#include <QMimeDatabase>
#include <QObject>
#include <QString>
#include <QStringList>
#include <qglobal.h>
#include <qtmetamacros.h>

namespace LosModel
{
class LosFilePath
{
  public:
    LosFilePath();
    LosFilePath(const QString &);
    LosFilePath(const LosFilePath &);
    ~LosFilePath() = default;

  public: // init
    void loadFile(const QString &file_path);

  public: // get
    const QString &getFilePath() const;
    const QString &getFileName() const;
    QString getAbsolutePath() const;
    QString getAbsoluteFilePath() const;
    QString getBaseFileName() const;
    QString getSuffix() const;
    LosCommon::LOS_ENUM_FileType getFileType() const;
    bool isDir() const;
    bool isFile() const;
    bool isExist() const;
    bool isBinary() const;

  public:
    void setFilePath(const QString &);

  protected:
    bool operator==(const QString &str) const;

  private:
    QString L_filePath;
    QString L_fileName;
    LosCommon::LOS_ENUM_FileType LOS_fileType;
};

QDebug operator<<(QDebug debug, const LosFilePath &path);
QStringList &operator<<(QStringList &, const LosFilePath &path);

}; // namespace LosModel