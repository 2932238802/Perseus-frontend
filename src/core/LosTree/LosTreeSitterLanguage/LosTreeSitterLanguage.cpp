
#include "LosTreeSitterLanguage.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"

namespace LosCore
{
    /**
     * @brief 判断是不是 支持 该语言
     *
     * @param language
     * @return true
     * @return false
     */
    bool LosTreeSitterLanguage::isSupported(LosCommon::LosToolChain_Constants::LosLanguage language)
    {
        using Language = LosCommon::LosToolChain_Constants::LosLanguage;
        switch (language)
        {
        case Language::CXX:
        {
            return true;
        }
        default:
            return false;
        }
    }

} // namespace LosCore