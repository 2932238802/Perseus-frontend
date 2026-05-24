#include "common/constants/ConstantsClass/LosToolChainClass.h"

namespace LosCommon
{

    /**
     * @brief stringToLanguage
     * 字符串 -> 具体枚举
     * @param str
     * @return LosCommon::LosToolChain_Constants::LosLanguage
     */
    inline LosCommon::LosToolChain_Constants::LosLanguage StrToLanguage(const QString &str)
    {
        using namespace LosCommon::LosToolChain_Constants;
        QString upperStr = str.toUpper();
        if (upperStr == "CXX")
            return LosLanguage::CXX;
        if (upperStr == "LUA")
            return LosLanguage::LUA;
        if (upperStr == "RUST")
            return LosLanguage::RUST;
        if (upperStr == "JAVA")
            return LosLanguage::JAVA;
        if (upperStr == "PYTHON")
            return LosLanguage::PYTHON;
        if (upperStr == "CSHARP")
            return LosLanguage::CSHARP;
        if (upperStr == "CMAKE")
            return LosLanguage::CMAKE;
        return LosLanguage::UNKNOWN;
    }

} // namespace LosCommon