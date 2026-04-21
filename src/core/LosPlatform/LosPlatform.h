#pragma once
#include "common/constants/ConstantsClass/LosPlatformClass.h"
#include <QFile>
#include <qdebug.h>
namespace LosCore
{

    class LosPlatform
    {
      private:
        static LosCommon::LosPlatform_Constants::OsType detectOs();
        static LosCommon::LosPlatform_Constants::OsType LOS_osType;

      public: /* tool */
        static LosCommon::LosPlatform_Constants::OsType getOs();
        static QString lineEnding();
    };
} /* namespace LosCore */