
#include "LosRunManager.h"
#include "core/LosRunner/LosSingleCppRunner/LosSingleCppRunner.h"
#include "models/LosFilePath/LosFilePath.h"

namespace LosCore
{
/**
构造函数

*/
LosRunManager::LosRunManager(QObject *parent) : QObject{parent}
{
    LOS_runner = nullptr;
}
LosRunManager::~LosRunManager()
{
    stop();
}

/**
执行函数
*/
void LosRunManager::execute(const QString &file_path)
{
    stop();

    LosModel::LosFilePath path(file_path);
    if (!path.isExist())
    {
        ERR("error, path does not exist!" + file_path, "LosRunManager");
        return;
    }

    if (path.isFile())
    {
        if (file_path.endsWith(".cpp") || file_path.endsWith(".cc"))
        {
            LOS_runner = new LosSingleCppRunner(file_path, this);
        }
        else
        {
            ERR("compiling this type of file is not currently supported", "LosRunManager");
        }
    }
    else if (path.isDir())
    {
        ERR(u8"folder-level overall builds (CMake) are not yet implemented!", "LosRunManager");
        return;
    }

    if (LOS_runner)
    {
        LOS_runner->start();
    }
}

/**
暂停
*/
void LosRunManager::stop()
{
    if (LOS_runner)
    {
        LOS_runner->stop();
        LOS_runner->deleteLater();
        LOS_runner = nullptr;
    }
}

} // namespace LosCore