#pragma once
/*
 * 格式化管理器参数配置
 */


namespace LosCommon
{
    namespace LosFormatManager_Constants
    {
        inline constexpr const char *ASSUME_FILENAME_ASRS = "-assume-filename=";
        inline constexpr const char *STYLE_ASRS           = "-style=";
#ifdef Q_OS_WIN
        inline constexpr const char *CLANG_FORMAT = "clang-format.exe";
#else
        inline constexpr const char *CLANG_FORMAT = "clang-format";
#endif
    } /* namespace LosFormatManager_Constants */
} // namespace LosCommmon
