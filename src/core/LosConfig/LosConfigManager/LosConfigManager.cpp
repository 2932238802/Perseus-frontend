#include "LosConfigManager.h"

namespace LosCore
{
LosConfigManager::LosConfigManager(QObject *parent) : QObject{parent}
{
    initConnect();
}



/**
分析项目结构
*/
void LosConfigManager::analyse(const QString &absolute_path)
{
    if (LOS_config)
        LOS_config->analyse(absolute_path);
}



/**
创建
*/
void LosConfigManager::create(const QString &file_path)
{
    LOS_config = LosConfigFactory::instance().create(file_path, this);
}



/**
判断是不是 在文件内
*/
bool LosConfigManager::isInFiles(const QString &file_name)
{
    if (LOS_config)
        return LOS_config->isInFiles(file_name);
    else
        return false;
}



/**
初始化 连接
*/
void LosConfigManager::initConnect() {}

} // namespace LosCore