// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <optional>

namespace LosCommon
{
    // 在给定目录列表里找第一个存在且可执行的 exeName
    // 返回绝对路径；找不到返回空字符串
    inline QString FindExecutableInDirs(const QString &exeName, const QStringList &dirs)
    {
        for (const QString &dir : dirs)
        {
            QDir d(dir);
            if (!d.exists())
            {
                continue;
            }
            QString path = d.absoluteFilePath(exeName);
            QFileInfo info(path);
            if (info.exists() && info.isExecutable())
            {
                return path;
            }
        }
        return QString();
    }


    /**
     * @brief 哪些 支持 版本 搜索
     * 
     * @param exeName 
     * @return true 
     * @return false 
     */
    inline bool SupportsVersionSuffix(const QString &exeName)
    {
        const QString lower = exeName.toLower();
        return lower.startsWith("clang") || lower.startsWith("gcc") || lower == "g++" || lower.startsWith("g++-");
    }



    /**
     * @brief 
     * 
     * @param exeName 
     * @return std::optional<QString> 
     */
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
            if (!toolsDir.cdUp())
            {
                break;
            }
        }

#if defined(Q_OS_LINUX)
        {
            const QStringList searchDirs = {
                "/usr/local/bin",
                "/usr/bin",
                "/bin",
                "/snap/bin",
                QDir::homePath() + "/.local/bin",
            };

            QString exact = FindExecutableInDirs(exeName, searchDirs);
            if (!exact.isEmpty())
            {
                return exact;
            }
            // 再尝试版本化变体
            // clangd -> clangd-21 / clangd-18 ... 
            // 仅对常见带版本号的工具
            if (SupportsVersionSuffix(exeName))
            {
                for (int major = 21; major >= 7; --major)
                {
                    const QString variant = exeName + "-" + QString::number(major);
                    QString v           = FindExecutableInDirs(variant, searchDirs);
                    if (!v.isEmpty())
                    {
                        return v;
                    }
                }
            }

            QString sysPath = QStandardPaths::findExecutable(exeName);
            if (!sysPath.isEmpty() && !sysPath.startsWith("/mnt/"))
            {
                return sysPath;
            }
            QString winFallback = sysPath;
            if (SupportsVersionSuffix(exeName))
            {
                for (int major = 21; major >= 15; --major)
                {
                    const QString variant = exeName + "-" + QString::number(major);
                    QString v           = QStandardPaths::findExecutable(variant);
                    if (!v.isEmpty() && !v.startsWith("/mnt/"))
                    {
                        return v;
                    }
                }
            }
            if (!winFallback.isEmpty())
            {
                return winFallback;
            }
            return std::nullopt;
        }
#else
        // Windows / macOS 维持原逻辑，靠 PATH 搜索
        {
            QString sysPath = QStandardPaths::findExecutable(exeName);
            if (!sysPath.isEmpty())
            {
                return sysPath;
            }
            return std::nullopt;
        }
#endif
    }

} /* namespace LosCommon */