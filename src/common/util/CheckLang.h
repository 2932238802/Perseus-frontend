// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "models/LosFilePath/LosFilePath.h"

namespace LosCommon
{
    inline LosCommon::LosToolChain_Constants::LosLanguage CheckLang(const QString &file_path)
    {
        // 与 GetFileType (LosFileType.h) 保持一致: 后缀统一转小写, 兼容 .CC/.CXX/.HPP
        const QString lower = file_path.toLower();
        if (lower.endsWith(".cpp") || lower.endsWith(".cc") || lower.endsWith(".cxx") || lower.endsWith(".c") ||
            lower.endsWith(".h") || lower.endsWith(".hpp"))
        {
            return LosCommon::LosToolChain_Constants::LosLanguage::CXX;
        }
        else if (lower.endsWith(".py"))
        {
            return LosCommon::LosToolChain_Constants::LosLanguage::PYTHON;
        }
        else if (LosModel::LosFilePath(file_path).getFileName() == "CMakeLists.txt")
        {
            return LosCommon::LosToolChain_Constants::LosLanguage::CMAKE;
        }
        else if (lower.endsWith(".rs"))
        {
            return LosCommon::LosToolChain_Constants::LosLanguage::RUST;
        }
        else if (lower.endsWith(".md"))
        {
            return LosCommon::LosToolChain_Constants::LosLanguage::MARKDOWN;
        }
        return LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN;
    }
} /* namespace LosCommon */