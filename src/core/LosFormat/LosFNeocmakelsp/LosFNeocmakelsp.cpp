#include "LosFNeocmakelsp.h"

namespace LosCore
{
    LosFNeocmakelsp::LosFNeocmakelsp(QObject *parent) : LosFormatBase(parent) {}
    bool LosFNeocmakelsp::format(QString *out, const QString &file_path, const QString &raw_content)
    {
        QProcess *L_process;
        if (raw_content.isEmpty())
        {
            WAR("the content to be formatted is empty", "LosFormatManage");
            return false;
        }

        auto opt = LosCommon::FindExePath("neocmakelsp");
        if (!opt)
        {
            ERR("Cannot find neocmakelsp executable", "LosFormatManage");
            return false;
        }

        QTemporaryFile tempFile(file_path + "_XXXXXX.cmake");
        if (!tempFile.open())
        {
            ERR("Failed to create temporary file for formatting", "LosFormatManage");
            return false;
        }
        tempFile.write(raw_content.toUtf8());
        tempFile.close();

        QStringList args;
        args << "format" << tempFile.fileName();

        L_process->start(*opt, args);
        if (!L_process->waitForStarted(LosCommon::LosFormatManager_Constants::WAITFORSTARTED_TIME_MS))
        {
            ERR("failed to start neocmakelsp", "LosFormatManager");
            return false;
        }
        if (!L_process->waitForFinished(LosCommon::LosFormatManager_Constants::WAITFORFINISHED_TIME_MS))
        {
            ERR("neocmakelsp timeout!", "LosFormatManager");
            L_process->kill();
            return false;
        }

        QByteArray output = L_process->readAllStandardOutput();
        QByteArray error  = L_process->readAllStandardError();

        if (!error.isEmpty() && output.isEmpty())
        {
            ERR("neocmakelsp format error: " + QString::fromUtf8(error), "LosFormatManager");
            return false;
        }
        QString formattedText = QString::fromUtf8(output);
        if (formattedText.isEmpty())
        {
            ERR("neocmakelsp returned empty output!", "LosFormatManager");
            return false;
        }
        *out = formattedText;
        SUC("CMake Code formatted successfully", "LosFormatManager");

        return true;
    }
}; /* namespace LosCore */