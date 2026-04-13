#pragma once
#include "common/constants/ConstantsClass.h"
#include <QString>

namespace LosCommon
{
    inline QString GetSettingKey(LosCommon::LosToolChain_Constants::LosLanguage lan)
    {
        return QString("ToolChain/Lan_%1").arg(static_cast<int>(lan));
    };
} // namespace LosCommon