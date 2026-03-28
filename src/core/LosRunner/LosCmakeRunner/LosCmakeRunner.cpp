
#include "LosCmakeRunner.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "models/LosFilePath/LosFilePath.h"
#include <optional>
#include <qglobal.h>
#include <qprocess.h>

namespace LosCore
{
LosCmakeRunner::LosCmakeRunner(QObject *parent)
{
    L_cmake    = new QProcess(this);
    L_runner   = new QProcess(this);
    L_cmakeExe = "";
    initConnect();
}

LosCmakeRunner::~LosCmakeRunner()
{
    stop();
}


void LosCmakeRunner::start(const QString &buildPath)
{
    if (L_cmakeExe.isEmpty())
    {
        return;
    }

    if (L_cmake->state() != QProcess::NotRunning)
    {
        stop();
    }

    L_cmake->setWorkingDirectory(buildPath);

    QStringList args;
    args << "--build" << ".";
    L_cmake->start(L_cmakeExe, args);
}



void LosCmakeRunner::stop()
{
    if (L_cmake->state() != QProcess::NotRunning)
    {
        L_cmake->kill();
        L_cmake->waitForFinished(1000);
    }
    if (L_runner->state() != QProcess::NotRunning)
    {
        L_runner->kill();
        L_runner->waitForFinished(1000);
    }
}


void LosCmakeRunner::setCMakeExe(const QString &file_path)
{
    L_cmakeExe = file_path;
}


/**
初始化链接
*/
void LosCmakeRunner::initConnect()
{
    connect(L_cmake, &QProcess::readyReadStandardError, this,
            [=]() { ERR(QString::fromLocal8Bit(L_cmake->readAllStandardError()), "LosCmakeRunner"); });
    connect(L_cmake, &QProcess::readyReadStandardOutput, this,
            [=]() { INF(QString::fromLocal8Bit(L_cmake->readAllStandardOutput()), "LosCmakeRunner"); });
    connect(L_cmake, &QProcess::finished, this,
            [=](int exitCode, QProcess::ExitStatus exitStatus)
            {
                if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit) {
                    
                }
                else
                {
                    ERR("cmake err!", "LosCmakeRunner");
                }
            });
    connect(L_runner, &QProcess::readyReadStandardError, this,
            [=]() { ERR(QString::fromLocal8Bit(L_cmake->readAllStandardError()), "LosCmakeRunner"); });
    connect(L_runner, &QProcess::readyReadStandardOutput, this,
            [=]() { INF(QString::fromLocal8Bit(L_cmake->readAllStandardOutput()), "LosCmakeRunner"); });
}


std::optional<QString> LosCmakeRunner::findExePath(const QString &build_path, const QString &targer_name)
{
    QString suffix = "";
#ifdef Q_OS_WIN
    suffix = ".exe"; // Windows 平台补充后缀
#endif

    QString fileName = targer_name + suffix;

    QStringList possibleDirs = {"", "Debug", "Release", "Bin"};
    QDir baseDir(build_path);
    for (const QString &sub : possibleDirs)
    {
        QString fullPath = baseDir.absoluteFilePath(sub.isEmpty() ? fileName : sub + "/" + fileName);
        QFileInfo fileInfo(fullPath);
        if (fileInfo.exists() && fileInfo.isExecutable())
        {
            return fullPath;
        }
    }
    return std::nullopt;
}

} // namespace LosCore