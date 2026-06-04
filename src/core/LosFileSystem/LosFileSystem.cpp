// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosFileSystem.h"
#include "core/LosRouter/LosRouter.h"
namespace LosCore
{

    LosFileSystem &LosFileSystem::instance()
    {
        static LosFileSystem s;
        return s;
    }



    bool LosFileSystem::createDir(const QString &path)
    {
        QDir dir;
        if (dir.mkpath(path))
        {
            INF("created new folder: " + path, "LosFileTreeUi");
            emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
            return true;
        }
        ERR("failed to create folder: " + path, "LosFileTreeUi");
        return false;
    }



    /**
     * @brief createFile
     *
     * @param file_path
     * @param data
     * @return true
     * @return false
     */
    bool LosFileSystem::createFile(const QString &file_path, const QByteArray &data)
    {
        QFileInfo fileInfo(file_path);
        QString dirPath = fileInfo.absolutePath();
        if (!createDir(dirPath))
            return false;

        QFile file(file_path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return false;
        }

        // 创建成功了
        qint64 writtenBytes = file.write(data);
        file.close();
        emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
        return writtenBytes == data.size();
    }



    /**
     * @brief removeFile 删除文件
     *
     * @param file_path
     * @return true
     * @return false
     */
    bool LosFileSystem::removeFile(const QString &file_path)
    {
        if (QFile::remove(file_path))
        {
            emit LosRouter::instance()._cmd_fileSystemChanged();
            return true;
        }
        return false;
    }



    /**
     * @brief removeDir 删除文件夹
     *
     * @param dir_path
     * @return true
     * @return false
     */
    bool LosFileSystem::removeDir(const QString &dir_path)
    {
        QDir dir(dir_path);
        if (dir.removeRecursively())
        {
            emit LosRouter::instance()._cmd_fileSystemChanged();
            return true;
        }
        return false;
    }



    /**
     * @brief renameFile 重命名文件和文件夹
     *
     * @param src
     * @param dst
     * @return true
     * @return false
     */
    bool LosFileSystem::renameFile(const QString &src, const QString &dst)
    {
        if (QFile::rename(src, dst))
        {
            return true;
        }
        return false;
    }
} /* namespace LosCore */