#pragma once
#include "common/constants/ConstantsClass.h"
#include "models/LosFilePath/LosFilePath.h"

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
    else if (LosModel::LosFilePath(file_path).getFileName() == "CMakeLists.txt")
    {
        return LosCommon::LosToolChain_Constants::LosLanguage::CMAKE;
    }
    return LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN;
}
} // namespace LosCommon