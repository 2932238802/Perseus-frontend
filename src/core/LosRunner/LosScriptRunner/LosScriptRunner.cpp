#include "LosScriptRunner.h"

namespace LosCore
{
    LosScriptRunner::LosScriptRunner(QObject *parent) : LosAbstractRunner(parent)
    {
        L_runner = new QProcess(this);
        initConnect();
    }

    LosScriptRunner::~LosScriptRunner()
    {
        stop();
    }


    void LosScriptRunner::start(const QString &main_file_path)
    {
        if (!L_workingDir.isEmpty())
        {
            L_runner->setWorkingDirectory(L_workingDir);
        }

#ifdef Q_OS_WIN
        QStringList winArgs;
        winArgs << "/c" << main_file_path;
        winArgs.append(L_args); /* 把用户填的参数加在后面 */
        L_runner->start("cmd.exe", winArgs);
#else
        QStringList linuxArgs;
        linuxArgs << main_file_path;
        linuxArgs.append(L_args);
        L_runner->start("bash", linuxArgs);
#endif
    }



    void LosScriptRunner::stop()
    {
        if (L_runner->state() != QProcess::NotRunning)
        {
            L_runner->kill();
        }
    }


    void LosScriptRunner::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_runScript, this,
                [this](const QString &script_path, const QString &working_dir, const QStringList &args)
                {
                    this->L_args       = std::move(args);
                    this->L_workingDir = std::move(working_dir);
                    this->start(script_path);
                });
        connect(L_runner, &QProcess::readyReadStandardOutput, this,
                [this]()
                {
                    QString msg{"\n==== result ===="};
                    while (L_runner->canReadLine())
                    {
                        QByteArray rawData = L_runner->readLine();
                        QString lineStr    = QString::fromLocal8Bit(rawData).trimmed();
                        if (!lineStr.isEmpty())
                        {
                            msg += QString("\n%1").arg(lineStr);
                        }
                    }
                    msg += "\n================\n";
                    INF(msg, "LosScriptRunner");
                });

        connect(L_runner, &QProcess::readyReadStandardError, this,
                [this]() { INF(QString::fromLocal8Bit(L_runner->readAllStandardError()), "LosScriptRunner"); });

        connect(L_runner, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus)
                { INF("process terminated (exit code: " + QString::number(exitCode) + ")", "LosScriptRunner"); });

        connect(L_runner, &QProcess::errorOccurred, this,
                [=](QProcess::ProcessError error) { ERR("The compiled program failed to start", "LosScriptRunner"); });
    }


} /* namespace LosCore */