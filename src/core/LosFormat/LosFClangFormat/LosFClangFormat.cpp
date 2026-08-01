// Copyright (c) 2026 LosAngelous (shengjie.lin)



#include "LosFClangFormat.h"
#include "common/constants/ConstantsNum/LosFormatManagerNum.h"
#include "common/constants/ConstantsStr/LLVM_formatStyle.h"
#include "common/constants/ConstantsStr/LosFormatManagerStr.h"
#include "common/constants/ConstantsStr/LosStateStr.h"
#include "common/util/FindExePath.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"

#include <QObject>
#include <QProcess>
namespace LosCore
{
    LosFClangFormat::LosFClangFormat(QObject *parent) : LosFormatBase(parent)
    {
        initConnect();
    }



    /**
     * @brief format
     *
     * @param out
     * @param file_path
     * @param raw_content
     * @return true
     * @return false
     */
    bool LosFClangFormat::format(QString *out, const QString &file_path, const QString &raw_content)
    {
        QProcess L_process;
        if (raw_content.isEmpty())
        {
            WAR("the content to be formatted is empty", "LosFormatManage");
            return false;
        }

        QStringList args;
        args << LosCommon::LosFormatManager_Constants::ASSUME_FILENAME_ASRS + file_path;
        args << L_formatStyle;
        auto opt = LosCommon::FindExePath(LosCommon::LosFormatManager_Constants::CLANG_FORMAT);
        if (!opt)
            return false;
        L_process.start(*opt, args);

        if (!L_process.waitForStarted(LosCommon::LosFormatManager_Constants::WAITFORSTARTED_TIME_MS))
        {
            ERR("failed to start clang-format", "LosFormatManager");
            return false;
        }

        const auto &inputData = raw_content.toUtf8();
        L_process.write(inputData);
        L_process.closeWriteChannel();

        if (!L_process.waitForFinished(LosCommon::LosFormatManager_Constants::WAITFORFINISHED_TIME_MS))
        {
            ERR("clang-format timeout!", "LosFormatManager");
            L_process.kill();
            return false;
        }
        QByteArray output = L_process.readAllStandardOutput();
        QByteArray error  = L_process.readAllStandardError();
        if (!error.isEmpty())
        {
            if (!output.isEmpty())
            {
                WAR("clang-format warning: " + QString::fromUtf8(error), "LosFormatManager");
            }
            else
            {
                ERR("clang-format error: " + QString::fromUtf8(error), "LosFormatManager");
                return false;
            }
        }
        QString formattedText = QString::fromUtf8(output);
        if (formattedText.isEmpty())
        {
            ERR("clang-format returned empty output!", "LosFormatManager");
            return false;
        }

        *out = formattedText;
        SUC("Code formatt successfully", "LosFormatManager");
        return true;
    }



    /**
     * @brief chs 设置 clangformat
     *
     * @param str
     */
    void LosFClangFormat::setClangFormat(const QString &str) noexcept
    {
        L_formatStyle = QString{"{"} + (str.isEmpty() ? LosCommon::LLVM_formatStyle::FORMAT_STYLE : str) + QString{"}"};
    }



    /**
     * @brief 初始化连接
     */
    void LosFClangFormat::initConnect() noexcept
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosRouter::_cmd_clangFormatSet, this, &LosFClangFormat::setClangFormat);
    }


} /* namespace LosCore */