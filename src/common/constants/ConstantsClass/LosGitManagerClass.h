// Copyright (c) 2026 LosAngelous (shengjie.lin)
#pragma once
#include <QString>
namespace LosCommon
{
    namespace LosGitManager_Constants
    {
        struct LosGitFileStatus
        {
            QString L_absoluteFilePath;
            unsigned int L_statusFlag;
        };
    } // namespace LosGitManager_Constants
} // namespace LosCommon