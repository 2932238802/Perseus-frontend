#pragma once
#include "common/constants/ConstantsClass.h"

namespace LosCommon
{
inline LosCommon::LosToolChain_Constants::LosLanguage CheckLang(const QString &file_path)
{
    if (file_path.endsWith(".cpp") || file_path.endsWith(".c") || file_path.endsWith(".h"))
    {
        return LosCommon::LosToolChain_Constants::LosLanguage::CXX;
    }
    else if (file_path.endsWith(".py"))
    {
        return LosCommon::LosToolChain_Constants::LosLanguage::PYTHON;
    }
    return LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN;
}
} // namespace LosCommon