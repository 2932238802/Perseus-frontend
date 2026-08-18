// Copyright (c) 2026 LosAngelous (shengjie.lin)
#pragma once

#include "common/constants/ConstantsNum/LosSessionNum.h"
#include "common/constants/ConstantsStr/LLVM_formatStyle.h"
#include <QString>
#include <QStringList>

namespace LosCommon
{
    /**
     * @brief LosSession_Constants
     * 保留 会话 的固定字段
     */
    namespace LosSession_Constants
    {
        struct AuthConfig
        {
            QString L_username = "";
            QString L_token    = "";
        };

        /**
         * @brief 项目 格式 参数
         */
        struct FormatConfig
        {
            int L_fontSize = DEFAULT_FONT_SIZE;
            QString L_clangFormat{LosCommon::LLVM_formatStyle::FORMAT_STYLE} ;
        };

        struct Config
        {
            QStringList L_curFilePaths{};
            QString L_curProDir       = "";
            QString L_curActiveFile   = "";
            QString L_themeName       = "dracula";
            QString L_fontName        = "";
            AuthConfig LOS_authConfig = {};
            FormatConfig LOS_formatConfig = {};
        };

    } /* namespace LosSession_Constants */


} // namespace LosCommon