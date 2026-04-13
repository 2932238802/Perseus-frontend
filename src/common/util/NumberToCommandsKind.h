#pragma once

#include "common/constants/ConstantsNum.h"

namespace LosCommon
{
    inline LosCommandUi_Constants::CommandsKind NumberToCommandsKind(int kind_id)
    {
        switch (kind_id)
        {
        case 1:
        {
            return LosCommandUi_Constants::CommandsKind::SCRIPT;
        }
        case 2:
        {
            return LosCommandUi_Constants::CommandsKind::THEME;
        }
        default:
        {
            return LosCommandUi_Constants::CommandsKind::UNKOWN;
        }
        }
    }
} // namespace LosCommon