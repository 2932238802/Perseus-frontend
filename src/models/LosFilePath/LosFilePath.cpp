// Copyright (c) 2026 LosAngelous (shengjie.lin)

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



    /**
     * @brief getFilePath 返回存入的数据
     *
     * @return const QString&
     */
    const QString &LosFilePath::getFilePath() const
    {
        return L_filePath;
    }



    /**
     * @brief getFileName 获取文件的名字 包括后缀
     *
     * @return const QString&
     */
    const QString &LosFilePath::getFileName() const
    {
        return L_fileName;
    }



    /**
     * @brief getBaseFileName 获取文件的名字 没有后缀
     *
     * @return QString
     */
    QString LosFilePath::getBaseFileName() const
    {
        return QFileInfo(L_filePath).baseName();
    };



    /**
     * @brief getSuffix 获取后缀
     *
     * @return QString
     */
    QString LosFilePath::getSuffix() const
    {
        return QFileInfo(L_filePath).suffix().toLower();
    }



    /**
     * @brief getAbsolutePath 获取完整的路径 这个是不含文件名的 只有文件夹的名字
     *
     * @return QString
     */
    QString LosFilePath::getAbsolutePath() const
    {
        return QFileInfo(L_filePath).absolutePath();
    };



    /**
     * @brief getAbsoluteFilePath 获取完整的路径
     *
     * @return QString
     */
    QString LosFilePath::getAbsoluteFilePath() const
    {
        return QFileInfo(L_filePath).absoluteFilePath();
    }



    /**
     * @brief getFileType 获取文件类型
     *
     * @return LosCommon::LOS_ENUM_FileType
     */
    LosCommon::LOS_ENUM_FileType LosFilePath::getFileType() const
    {
        return LOS_fileType;
    }



    /**
     * @brief isDir 判断是不是文件夹
     *
     * @return true
     * @return false
     */
    bool LosFilePath::isDir() const
    {
        return QFileInfo(L_filePath).isDir();
    }



    /**
     * @brief isFile 哦啊
     *
     * @return true
     * @return false
     */
    bool LosFilePath::isFile() const
    {
        return QFileInfo(L_filePath).isFile();
    }



    /** 
     * @brief isExist 是否存在
     *
     * @return true
     * @return false
     */
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