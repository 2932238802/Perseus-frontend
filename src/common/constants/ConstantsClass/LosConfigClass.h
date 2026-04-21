
#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

namespace LosCore
{
    class LosConfig;
}

namespace LosCommon
{
    namespace LosConfig_Constants
    {
        struct ConfigItem
        {
            QString L_name;
            QStringList L_filesToHotUpdate;
            std::function<bool(const QString &)> L_sniffer;
            std::function<LosCore::LosConfig *(QObject *)> L_creator;
        };
    } /* namespace LosConfig_Constants */
} // namespace LosCommon