#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"

namespace LosCommon
{

    /*
     * - 字符串 转 具体工具
     */
    inline LosCommon::LosToolChain_Constants::LosTool StrToTool(const QString &str)
    {
        using namespace LosCommon::LosToolChain_Constants;
        QString s = str.toLower();
        if (s == "cmake")
            return LosTool::CMAKE;
        if (s == "ninja")
            return LosTool::NINJA;
        if (s == "git")
            return LosTool::GIT;
        if (s == "g++" || s == "g_plus_plus")
            return LosTool::G_PLUS_PLUS;
        if (s == "clangd")
            return LosTool::CLANGD;
        if (s == "clang-format" || s == "clang_format")
            return LosTool::CLANG_FORMAT;
        if (s == "cargo")
            return LosTool::CARGO;
        if (s == "rust-analyzer")
            return LosTool::RUST_ANALYZER;
        if (s == "neocmakelsp" || s == "neocmakelsp.exe")
            return LosTool::NEOCMAKELSP;
        if (s == "rustc")
            return LosTool::RUSTC;
        if (s == "python")
            return LosTool::PYTHON;
        if (s == "pyright" || s == "pyright-langserver")
            return LosTool::PYRIGHT;
        return LosTool::UNKNOWN;
    }

} // namespace LosCommon
