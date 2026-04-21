

#pragma once
#include "common/constants/ConstantsNum/LosCommandUiNum.h"
#include <QString>
#include <QStringList>

namespace LosCommon
{
    namespace LosCommandUi_Constants
    {
        enum CommandsKind
        {
            SCRIPT = 1,
            THEME  = 2,
            UNKOWN = -1,
        };

        struct CommandsInfo
        {
            LosCommon::LosCommandUi_Constants::CommandsKind kind =
                LosCommon::LosCommandUi_Constants::CommandsKind::UNKOWN;
            QString L_scriptPath;
            QStringList L_params;
        };
    } /* namespace LosCommandUi_Constants */

} // namespace LosCommon