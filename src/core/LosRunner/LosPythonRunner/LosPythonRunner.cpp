
#include "LosPythonRunner.h"


namespace LosCore
{
    /**
     * @brief Construct a new Los Python Runner:: Los Python Runner object
     *
     * @param parent
     */
    LosPythonRunner::LosPythonRunner(QObject *parent) : LosAbstractRunner(parent)
    {
        L_runner = new QProcess(this);
        initConnect();
    };
    LosPythonRunner::~LosPythonRunner()
    {
        stop();
    }



    /**
     * @brief start
     *
     * @param main_file_path
     */
    void LosPythonRunner::start(const QString &main_file_path)
    {
        SUC("start...", "LosPythonRunner");

        LOS_targetFile.loadFile(main_file_path);
        if (!LOS_targetFile.isExist())
        {
            ERR("file is not exist!", "LosPythonRunner");
            return;
        }
        if (!LOS_targetFile.isFile())
        {
            ERR("is not file!", "LosPythonRunner");
            return;
        }
        L_runner->setWorkingDirectory(LOS_targetFile.getAbsolutePath());
        QStringList args;
        args << LOS_targetFile.getAbsoluteFilePath();
        L_runner->start(L_exePath, args);
    }



    /**
     * @brief
     *
     */
    void LosPythonRunner::stop()
    {
        if (L_runner->state() != QProcess::NotRunning)
        {
            L_runner->kill();
            L_runner->waitForFinished(1000);
        }
    }



    /**
     * @brief
     *
     * @param file_path
     */
    void LosPythonRunner::setExePath(const QString &file_path)
    {
        L_exePath = std::move(file_path);
        LosState::instance().set<QString>(LosCommon::LosState_Constants::SG_STR::PYTHON_EXE_PATH, file_path);
    }



    /**
     * @brief initConnect
     *
     */
    void LosPythonRunner::initConnect()
    {
        connect(L_runner, &QProcess::readyReadStandardError, this,
                [this]() { ERR(QString::fromLocal8Bit(L_runner->readAllStandardError()), "LosPythonRunner"); });
        connect(L_runner, &QProcess::readyReadStandardOutput, this,
                [this]() { INF(QString::fromLocal8Bit(L_runner->readAllStandardOutput()), "LosPythonRunner"); });
        connect(L_runner, &QProcess::finished, this,
                [this](int exitCode, QProcess::ExitStatus exitStatus)
                {
                    if (exitStatus == QProcess::NormalExit)
                    {
                        INF(QString("process finished with exit code %1").arg(exitCode), "LosPythonRunner");
                    }
                    else
                    {
                        ERR(QString("process crashed or was killed! (Exit code: %1)").arg(exitCode), "LosPythonRunner");
                    }
                });
        connect(L_runner, &QProcess::errorOccurred, this,
                [this](QProcess::ProcessError err) { INF("LosPythonRunner error: " + QString::number(err), "LosPythonRunner"); });
    }

} // namespace LosCore