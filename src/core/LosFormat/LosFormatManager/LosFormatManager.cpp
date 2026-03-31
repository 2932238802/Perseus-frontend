
#include "LosFormatManager.h"
#include "models/LosFilePath/LosFilePath.h"
#include <qfileinfo.h>

namespace LosCore
{

LosFormatManager::LosFormatManager(QObject *parent) : QObject(parent) {}

LosFormatManager &LosFormatManager::instance()
{
    static LosFormatManager l;
    return l;
}



/**
格式化文本
- 暂时只是 处理 C++ 文件
*/
bool LosFormatManager::format(QString *out, const QString &file_path, const QString &raw_content)
{
    auto sfx = LosModel::LosFilePath(file_path).getSuffix();
    if (sfx != "cpp" || sfx != "cc" || sfx != "c")
    {
        *out = raw_content;
        return true;
    }

    if (raw_content.isEmpty())
    {
        WAR("the content to be formatted is empty", "LosFormatManage");
        return false;
    }

    QProcess process;
    QStringList args;

    args << LosCommon::LosFormatManager_Constants::ASSUME_FILENAME_ASRS + file_path;
    args << LosCommon::LosFormatManager_Constants::STYLE_ASRS + QString(LosCommon::LLVM_formatStyle::FORMAT_STYLE);

    process.start(LosCommon::LosFormatManager_Constants::CLANG_FORMAT, args);

    if (!process.waitForStarted(LosCommon::LosFormatManager_Constants::WAITFORSTARTED_TIME_MS))
    {
        ERR("failed to start clang-format", "LosFormatManager");
        return false;
    }

    const auto &inputData = raw_content.toUtf8();
    process.write(inputData);
    process.closeWriteChannel();

    if (!process.waitForFinished(LosCommon::LosFormatManager_Constants::WAITFORFINISHED_TIME_MS))
    {
        ERR("clang-format timeout!", "LosFormatManager");
        process.kill();
        return false;
    }

    QByteArray output = process.readAllStandardOutput();
    QByteArray error  = process.readAllStandardError();

    if (!error.isEmpty())
    {
        if (!output.isEmpty())
        {
            WAR("clang-format warning: " + QString::fromUtf8(error), "LosFormatManager");
        }
        else
        {
            ERR("clang-format error: " + QString::fromUtf8(error), "LosFormatManager");
            return false;
        }
    }

    QString formattedText = QString::fromUtf8(output);

    if (formattedText.isEmpty())
    {
        ERR("clang-format returned empty output!", "LosFormatManager");
        return false;
    }

    *out = formattedText;
    SUC("Code formatt successfully", "LosFormatManager");
    return true;
}



} // namespace LosCore