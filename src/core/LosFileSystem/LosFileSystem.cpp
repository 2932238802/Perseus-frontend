
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



bool LosFileSystem::createFile(const QString &file_path, const QByteArray &data)
{
    QFileInfo fileInfo;
    QString dirPath = fileInfo.absolutePath();
    if (!createDir(dirPath))
        return false;

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    // 创建成功了
    emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();

    qint64 writtenBytes = file.write(data);
    file.close();
    return writtenBytes == data.size();
}



bool LosFileSystem::removeFile(const QString &file_path)
{
    if (QFile::remove(file_path))
    {
        emit LosRouter::instance()._cmd_fileSystemChanged();
        return true;
    }
    return false;
}



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



bool LosFileSystem::renameFile(const QString &src, const QString &dst)
{
    if (QFile::rename(src, dst))
    {
        emit LosRouter::instance()._cmd_fileSystemChanged();
        return true;
    }
    return false;
}
} // namespace LosCore