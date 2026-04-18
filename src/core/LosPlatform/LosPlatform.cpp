

#include "LosPlatform.h"


namespace LosCore
{
    LosCommon::LosPlatform_Constants::OsType LosPlatform::LOS_osType = LosCommon::LosPlatform_Constants::OsType::UNKOWN;



    LosCommon::LosPlatform_Constants::OsType LosPlatform::detectOs()
    {
#ifdef Q_OS_WIN
        return LosCommon::LosPlatform_Constants::OsType::WINDOWS;
#elif defined(Q_OS_MAC)
        return LosCommon::LosPlatform_Constants::OsType::MACOS;
#elif defined(Q_OS_LINUX)
        QFile versionFIle("/proc/version");
        if (versionFIle.open(QIODevice::ReadOnly))
        {
            QString versionInfo = versionFIle.readAll().toLower();
            versionFIle.close();
            if (versionInfo.contains("microsoft") || versionInfo.contains("wsl"))
            {
                return LosCommon::LosPlatform_Constants::OsType::LINUX_W;
            }
        }
        return LosCommon::LosPlatform_Constants::OsType::LINUX_N;
#else
        return LosCommon::LosPlatform_Constants::OsType::UNKOWN;
#endif
    }



    LosCommon::LosPlatform_Constants::OsType LosPlatform::getOs()
    {
        if (LOS_osType == LosCommon::LosPlatform_Constants::OsType::UNKOWN)
        {
            LOS_osType = detectOs();
        }
        return LOS_osType;
    }


    QString LosPlatform::lineEnding()
    {
        return getOs() == LosCommon::LosPlatform_Constants::OsType::WINDOWS ? "\r\n" : "\n";
    }
} /* namespace LosCore */