#pragma once

namespace LosCommon
{
    /*
     * 单文件的运行配置参数
     */
    namespace LosRunner_Constants
    {
        inline constexpr const char *CXX_17       = "-std=c++17";
        inline constexpr const char *CMD_OBJECT   = "-o";
        inline constexpr const char *LINUX_EXE    = ".out";
        inline constexpr const char *WIN_EXE      = ".exe";
        inline constexpr const char *OUTPUT_BUILD = "output";
        inline constexpr const char *OUTPUT_GXX   = "gxx";
        inline constexpr const char *OUTPUT_RUSTC = "rustc";
    } /* namespace LosRunner_Constants */
}