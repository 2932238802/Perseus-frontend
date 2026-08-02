// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QtGlobal>

namespace LosCommon
{
    namespace LosEncodingDetector_Constants
    {
        inline constexpr const char *BOM_HEX_UTF32_LE = "FFFE0000";
        inline constexpr const char *BOM_HEX_UTF32_BE = "0000FEFF";
        inline constexpr const char *BOM_HEX_UTF16_LE = "FFFE";
        inline constexpr const char *BOM_HEX_UTF16_BE = "FEFF";
        inline constexpr const char *BOM_HEX_UTF8     = "EFBBBF";

        inline constexpr const char *ENC_UNKNOWN  = "Unknown";
        inline constexpr const char *ENC_UTF8     = "UTF-8";
        inline constexpr const char *ENC_UTF8_BOM = "UTF-8 with BOM";
        inline constexpr const char *ENC_UTF16_LE = "UTF-16 LE";
        inline constexpr const char *ENC_UTF16_BE = "UTF-16 BE";
        inline constexpr const char *ENC_UTF32_LE = "UTF-32 LE";
        inline constexpr const char *ENC_UTF32_BE = "UTF-32 BE";
        inline constexpr const char *ENC_GBK      = "GBK";
        inline constexpr const char *ENC_GB18030  = "GB18030";

        inline constexpr char16_t CJK_RANGE_MIN = 0x4E00;
        inline constexpr char16_t CJK_RANGE_MAX = 0x9FFF;

        inline constexpr qint64 SAMPLE_BYTES = 8 * 1024;

        inline constexpr double CJK_MIN_RATIO = 0.30;

    }; /* namespace LosEncodingDetector_Constants */
} // namespace LosCommon
