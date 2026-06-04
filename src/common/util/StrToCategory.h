// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "common/constants/ConstantsClass/LosToolChainClass.h"

namespace LosCommon
{

    /**
     * @brief stringToCategory
     * 字符串 转 具体 枚举
     *
     * @param str
     * @return LosCommon::LosToolChain_Constants::ToolCategory
     */
    inline LosCommon::LosToolChain_Constants::ToolCategory StrToCategory(const QString &str)
    {
        using namespace LosCommon::LosToolChain_Constants;
        QString upperStr = str.toUpper();
        if (upperStr == "LSP")
            return ToolCategory::LSP;
        if (upperStr == "COMPILER")
            return ToolCategory::Compiler;
        if (upperStr == "FORMATTER")
            return ToolCategory::Formatter;
        if (upperStr == "LINTER")
            return ToolCategory::Linter;
        if (upperStr == "DEBUGGER")
            return ToolCategory::Debugger;
        if (upperStr == "BUILDTOOL")
            return ToolCategory::BuildTool;
        return ToolCategory::Compiler;
    }

} // namespace LosCommon