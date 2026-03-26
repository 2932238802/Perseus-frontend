#include "LosConfigFactory.h"

namespace LosCore
{
LosConfigFactory &LosConfigFactory::instance()
{
    static LosConfigFactory lcf;
    return lcf;
}



LosCore::LosConfig *LosConfigFactory::create(const QString &file_path, QObject *parent)
{
    for (auto a : L_configItems)
    {
        if (a.L_sniffer(file_path))
        {
            return a.L_creator(parent);
        }
    }
    return nullptr;
}

} // namespace LosCore