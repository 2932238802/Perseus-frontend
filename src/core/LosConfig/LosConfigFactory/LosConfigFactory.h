

#pragma once

#include "common/constants/ConstantsClass.h"
#include "core/LosConfig/LosConfig/LosConfig.h"
#include <QString>
#include <vector>
namespace LosCore
{

class LosConfigFactory
{
  public:
    static LosConfigFactory &instance();
    template <class LosConfig> void regis(const QString &name)
    {
        LosCommon::LosConfig_Constants::ConfigItem item;
        item.L_name             = name;
        item.L_creator          = [](QObject *parent) { return new LosConfig(parent); };
        item.L_sniffer          = &LosConfig::can;
        item.L_filesToHotUpdate = LosConfig::files;
        L_configItems.emplace_back(std::move(item));
    }
    LosCore::LosConfig *create(const QString &file_path, QObject *parent);

  private:
    LosConfigFactory()  = default;
    ~LosConfigFactory() = default;

  private:
    std::vector<LosCommon::LosConfig_Constants::ConfigItem> L_configItems;
};


template <class LosConfig> struct regis_t
{
    regis_t(const QString &absolute_path) { LosConfigFactory::instance().regis<LosConfig>(absolute_path); };
};


} // namespace LosCore