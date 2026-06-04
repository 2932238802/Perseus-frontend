// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosPlatformClass.h"

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