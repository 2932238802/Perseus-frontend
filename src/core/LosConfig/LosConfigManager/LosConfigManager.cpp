#include "LosConfigManager.h"
#include "core/LosConfig/LosConfigFactory/LosConfigFactory.h"
#include <qstringliteral.h>

namespace LosCore
{

LosConfigManager::LosConfigManager(QObject *parent) : QObject{parent}
{
    initConnect();
}


void LosConfigManager::analyse(const QString &absolute_path)
{
    if (LOS_config)
        LOS_config->analyse(absolute_path);
}


void LosConfigManager::create(const QString &file_path)
{
    LOS_config = LosConfigFactory::instance().create(file_path, this);
}


bool LosConfigManager::isInFiles(const QString &file_name)
{
    if (LOS_config)
        return LOS_config->isInFiles(file_name);
    else
        return false;
}


void LosConfigManager::initConnect() {}

} // namespace LosCore