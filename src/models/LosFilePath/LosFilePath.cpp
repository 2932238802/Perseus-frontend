#include "LosFilePath.h"
#include <qfileinfo.h>
#include <qmimedatabase.h>
#include <qmimetype.h>

namespace LosModel
{
    /*
     * 构造和析构函数
     */
    LosFilePath::LosFilePath() : L_fileName(""), L_filePath("") {}

    LosFilePath::LosFilePath(const QString &file_path)
    {
        loadFile(file_path);
    }
    LosFilePath::LosFilePath(const LosFilePath &file)
        : L_filePath(file.L_filePath), L_fileName(file.L_fileName), LOS_fileType(file.LOS_fileType)
    {
        loadFile(L_filePath);
    }



    /*
     * 加载文件
     */
    void LosFilePath::loadFile(const QString &file_path)
    {
        L_filePath = QDir::fromNativeSeparators(file_path);
        QFileInfo fileInfo(L_filePath);
        L_fileName = QFileInfo(L_filePath).fileName();

        if (fileInfo.isDir())
        {
            LOS_fileType = LosCommon::LOS_ENUM_FileType::FT_FOLDER;
        }
        else if (fileInfo.isFile())
        {
            LOS_fileType = LosCommon::GetFileType(fileInfo.suffix());
        }
        else
        {
            LOS_fileType = LosCommon::LOS_ENUM_FileType::FT_OTHER;
            WAR("unrecognized file type ->" + file_path, "LosFilePath");
        }
    }



    const QString &LosFilePath::getFilePath() const
    {
        return L_filePath;
    }



    const QString &LosFilePath::getFileName() const
    {
        return L_fileName;
    }



    QString LosFilePath::getBaseFileName() const
    {
        return QFileInfo(L_filePath).baseName();
    };



    QString LosFilePath::getSuffix() const
    {
        return QFileInfo(L_filePath).suffix().toLower();
    }



    QString LosFilePath::getAbsolutePath() const
    {
        return QFileInfo(L_filePath).absolutePath();
    };



    QString LosFilePath::getAbsoluteFilePath() const
    {
        return QFileInfo(L_filePath).absoluteFilePath();
    }



    LosCommon::LOS_ENUM_FileType LosFilePath::getFileType() const
    {
        return LOS_fileType;
    }



    bool LosFilePath::isDir() const
    {
        return QFileInfo(L_filePath).isDir();
    }



    bool LosFilePath::isFile() const
    {
        return QFileInfo(L_filePath).isFile();
    }



    bool LosFilePath::isExist() const
    {
        return QFileInfo(L_filePath).exists();
    }



    /*
     * - 文本文件”和“二进制文件
     * - 用来标识文件或数据内容格式的一种标准方式
     */
    bool LosFilePath::isBinary() const
    {
        static QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(L_filePath);
        return !type.inherits("text/plain");
    }



    void LosFilePath::setFilePath(const QString &file)
    {
        loadFile(file);
    }


    QDebug operator<<(QDebug debug, const LosFilePath &path)
    {
        QDebugStateSaver saver(debug);
        debug.nospace();
        debug << "LosFilePath(" << "Name: \"" << path.getFileName() << "\", " << "Path: \"" << path.getFilePath()
              << "\", " << "Type: " << (path.isDir() ? "Folder" : "File") << ", "
              << "Exists: " << (path.isExist() ? "True" : "False") << ")";
        return debug;
    }



    /*
     * 支持 QStringList的流式操作
     *
     */
    QStringList &operator<<(QStringList &list, const LosFilePath &path)
    {
        list.append(path.getFilePath());
        return list;
    }



    /*
     * 对比
     */
    bool LosFilePath::operator==(const QString &str) const
    {
        return L_filePath == str;
    }

} /* namespace LosModel */