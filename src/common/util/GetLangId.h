// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include <QString>

namespace LosCommon
{
    inline QString getLangId(LosCommon::LosToolChain_Constants::LosLanguage lang)
    {
        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            return "cpp";
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::PYTHON:
        {
            return "python";
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::CMAKE:
        {
            return "cmake";
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
        {
            return "rust";
        }
        default:
            return "plaintext";
        }
    }
} // namespace LosCommon