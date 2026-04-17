#pragma once
#include "common/constants/ConstantsClass.h"
#include "core/LosLog/LosLog.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <optional>
#include <qcoreapplication.h>
#include <qfileinfo.h>
#include <typeindex>

namespace LosCommon
{
    template <LosCommon::FindFileType type>
    inline std::optional<QString> GetFilePathFromUp(const QString &base_file_name, int max_times = 5)
    {
        INF("find ... :" + base_file_name, "GetFilePathFromUp");
        QDir dir(QCoreApplication::applicationDirPath());

        QString targetPath = base_file_name;

        if constexpr (type == LosCommon::FindFileType::SYSTEM_TOOLCHAIN_CONFIG_JSON)
        {
            targetPath = "resources/config/" + base_file_name;
        }

        for (int i = 0; i < max_times; i++)
        {
            if (dir.exists(targetPath))
            {
                QString fullPath = dir.filePath(targetPath);
                QFileInfo fileInfo(fullPath);
                
                if constexpr (type == LosCommon::FindFileType::COMMON_DIR)
                {
                    if (fileInfo.isDir()) return fullPath;
                }
                else 
                {
                    if (fileInfo.isFile()) return fullPath;
                }
            }
            if (!dir.cdUp())
                break;
        }
        return std::nullopt;
    }
} /* namespace LosCommon */
