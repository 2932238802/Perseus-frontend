
#include "LosFormatManager.h"


namespace LosCore
{

    LosFormatManager::LosFormatManager(QObject *parent) : QObject(parent) {}

    LosFormatManager &LosFormatManager::instance()
    {
        static LosFormatManager l;
        return l;
    }



    /*
     * 格式化文本
     * - 暂时只是 处理 C++ 文件
     */
    bool LosFormatManager::format(QString *out, const QString &file_path, const QString &raw_content)
    {
        auto lang = LosCommon::CheckLang(file_path);
        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            // clang-format
            if (!L_formats.contains(LosCommon::LosToolChain_Constants::LosLanguage::CXX))
            {
                L_formats[LosCommon::LosToolChain_Constants::LosLanguage::CXX] = new LosFClangFormat(this);
            }
            return L_formats[LosCommon::LosToolChain_Constants::LosLanguage::CXX]->format(out, file_path, raw_content);
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::CMAKE:
        {
            /*
             * neocmakelsp
             */
            if (!L_formats.contains(LosCommon::LosToolChain_Constants::LosLanguage::CMAKE))
            {
                L_formats[LosCommon::LosToolChain_Constants::LosLanguage::CMAKE] = new LosFNeocmakelsp(this);
            }
            return L_formats[LosCommon::LosToolChain_Constants::LosLanguage::CMAKE]->format(out, file_path,
                                                                                            raw_content);
        }
        default:
            return false;
        }
    }



} /* namespace LosCore */