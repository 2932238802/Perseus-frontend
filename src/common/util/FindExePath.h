#pragma once
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QString>
#include <optional>

namespace LosCommon
{
inline std::optional<QString> FindExePath(const QString &exeName)
{
    QString osSubDir;
#if defined(Q_OS_WIN)
    osSubDir = "windows";
#elif defined(Q_OS_MAC)
    osSubDir = "macos";
#else
    osSubDir = "linux";
#endif
    QString appDir = QCoreApplication::applicationDirPath();
    QDir toolsDir(appDir);
    for (int i = 0; i < 5; i++)
    {
        QString bundledPath = toolsDir.absoluteFilePath("tools/" + osSubDir + "/" + exeName);
        QFileInfo bundledFile(bundledPath);
        if (bundledFile.exists() && bundledFile.isExecutable())
        {
            return bundledPath;
        }
        QString sysPath = QStandardPaths::findExecutable(exeName);
        if (!sysPath.isEmpty())
        {
#ifdef Q_OS_LINUX
            if (sysPath.startsWith("/mnt/c/") || sysPath.startsWith("/mnt/d/") || sysPath.startsWith("/mnt/"))
            {
                return std::nullopt;
            }
#endif
            return sysPath;
        }
        if (!toolsDir.cdUp())
        {
            break;
        }
    }

    return std::nullopt;
}

} // namespace LosCommon
