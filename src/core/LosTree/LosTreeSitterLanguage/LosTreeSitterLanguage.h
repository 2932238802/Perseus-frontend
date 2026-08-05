#pragma once

#include "common/constants/ConstantsClass/LosToolChainClass.h"
namespace LosCore
{
    /**
     * @brief 语言解析器
     * 
     */
    class LosTreeSitterLanguage
    {
      public:
        static bool isSupported(LosCommon::LosToolChain_Constants::LosLanguage language);
    };
} // namespace LosCore