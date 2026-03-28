
#include "LosRunManager.h"
#include "common/constants/ConstantsClass.h"
#include "common/util/CheckLang.h"

#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "core/LosRunner/LosSingleCppRunner/LosSingleCppRunner.h"
#include "core/log/LosLog/LosLog.h"

#include <qsharedpointer.h>

namespace LosCore
{
/**
构造函数

*/
LosRunManager::LosRunManager(QObject *parent) : QObject{parent}
{
    initConnect();
}
LosRunManager::~LosRunManager() {}

/**
执行函数
*/
void LosRunManager::execute(const QString &file_path, bool is_project)
{
    auto lang = LosCommon::CheckLang(file_path);
    if (!LOS_runners.contains(lang))
    {
        L_mainEntryFilePath = file_path;
        emit LosCore::LosRouter::instance()._cmd_checkToolchain(lang);
        WAR("runner is not init!", "LosRunManager");
        return;
    }
    if (auto runner = getCurRunner(file_path))
    {
        runner->start(file_path);
    }
}



void LosRunManager::stop()
{
    for (auto *runner : LOS_runners)
    {
        if (runner)
        {
            runner->stop();
        }
    }
}


void LosRunManager::onToolChainReady(LosCommon::LosToolChain_Constants::LosLanguage lan, const QString &exePath)
{
    if (!LOS_runners.contains(lan))
    {
        switch (lan)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            // 这里 可以 之后 加一个 是不是 项目模式 项目模式 可以用户手动 调
            LOS_runners[lan] = new LosSingleCppRunner(this);
            auto runner      = qobject_cast<LosSingleCppRunner *>(LOS_runners[lan]);
            runner->setExePath(exePath);
            execute(L_mainEntryFilePath);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::PYTHON:
        {
            break;
        }
        default:
        {
            break;
        }
        }
    }
}


/**
获取 当前的 执行器
*/
LosAbstractRunner *LosRunManager::getCurRunner(const QString &file_path)
{
    auto lang = LosCommon::CheckLang(file_path);
    return LOS_runners.value(lang, nullptr);
}

/**
连接
*/
void LosRunManager::initConnect()
{
    auto &router = LosCore::LosRouter::instance();
    connect(&router, &LosCore::LosRouter::_cmd_toolChainReady, this, &LosRunManager::onToolChainReady);
}


} // namespace LosCore