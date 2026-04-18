#pragma once

namespace LosCommon
{
    /*
     * CMake 构建系统配置常量
     */
    namespace LosConfig_Constants
    {
        inline constexpr const char *CMAKE                   = "cmake";
        inline constexpr const char *SRC_DIR                 = ".";
        inline constexpr const char *CMD_SOURCE              = "-S";
        inline constexpr const char *CMD_TARGET              = "-B";
        inline constexpr const char *BUILD_NAME              = "build";
        inline constexpr const char *COMPILE_COMMANDS_OPTION = "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON";
        inline constexpr const char *COMPILE_COMMANDS_FILE   = "compile_commands.json";
    } /* namespace LosConfig_Constants */


} // namespace LosCommon