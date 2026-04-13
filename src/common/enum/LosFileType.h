#pragma once
#include <QString>
#include <regex>

namespace LosCommon
{

    /*
     * 文件类型
     */
    enum class LOS_ENUM_FileType
    {
        FT_CPP = 0,
        FT_H,
        FT_C,
        FT_TXT,
        FT_OTHER,
        FT_MD,
        FT_FOLDER,
    };

    inline LOS_ENUM_FileType GetFileType(const QString &suffix)
    {
        QString suffix_to_lower = suffix.toLower();
        std::regex rCpp(R"(^(cpp|cc|cxx)$)");
        std::regex rH(R"(hpp|h)");

        if (std::regex_match(suffix_to_lower.toStdString(), rCpp))
        {
            return LOS_ENUM_FileType::FT_CPP;
        }
        else if (std::regex_match(suffix_to_lower.toStdString(), rH))
        {
            return LOS_ENUM_FileType::FT_H;
        }
        else if ("c" == suffix)
        {
            return LOS_ENUM_FileType::FT_C;
        }
        else if ("md" == suffix)
        {
            return LOS_ENUM_FileType::FT_MD;
        }
        else if ("txt")
        {
            return LOS_ENUM_FileType::FT_TXT;
        }
        else
        {
            return LOS_ENUM_FileType::FT_OTHER;
        }
    }
} // namespace LosCommon