// Copyright (c) 2026 LosAngelous (shengjie.lin)


#pragma once
#include "common/constants/ConstantsNum/LosSessionNum.h"
#include <QString>
#include <QStringList>

namespace LosCommon
{


    /*
     * - 保留 会话 的固定字段
     */
    namespace LosSession_Constants
    {
        struct AuthConfig
        {
            QString L_username = "";
            QString L_token    = "";
        };

        struct SizeConfig
        {
            int L_fontSize = DEFAULT_FONT_SIZE;
        };

        struct Config
        {
            QStringList L_curFilePaths{};
            QString L_curProDir       = "";
            QString L_curActiveFile   = "";
            QString L_themeName       = "dracula";
            QString L_fontName        = "";
            AuthConfig LOS_authConfig = {};
            SizeConfig LOS_sizeConfig = {};
        };

    } /* namespace LosSession_Constants */


} // namespace LosCommon