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

    QString targetPath = "";

    // 编译器 分支
    if constexpr (type == LosCommon::FindFileType::COMMON)
    {
        targetPath = base_file_name;
    }
    else if constexpr (type == LosCommon::FindFileType::SYSTEM_TOOLCHAIN_CONFIG_JSON)
    {
        targetPath = "resources/config/" + base_file_name;
    }
    for (int i = 0; i < max_times; i++)
    {
        // QDir 这里的 exist 就是判断一下 当前文件夹 存不存在 指定的文件
        if (dir.exists(targetPath))
        {
            return dir.filePath(targetPath);
        }
        if (!dir.cdUp())
            break;
    }
    return std::nullopt;
}


} // namespace LosCommon