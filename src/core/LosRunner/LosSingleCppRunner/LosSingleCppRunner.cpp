
#include "LosSingleCppRunner.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include <qglobal.h>
#include <qobject.h>
#include <qprocess.h>
#include <qtmetamacros.h>

namespace LosCore
{

/**
默认构造和初始化
*/
LosSingleCppRunner::LosSingleCppRunner(const QString &file_path, QObject *parent) : LosAbstractRunner{parent}
{
    LOS_filePath.loadFile(file_path);
    L_gxxPro = new QProcess(this);
    L_runPro = new QProcess(this);

#ifdef Q_OS_WIN
    L_exePath = LOS_filePath.getAbsolutePath() + "/" + LOS_filePath.getBaseFileName() + LosCommon::WIN_EXE;
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    L_exePath = LOS_filePath.getAbsolutePath() + "/" + LOS_filePath.getBaseFileName() + LosCommon::LINUX_EXE;
#else
    L_exePath = LOS_filePath.getAbsolutePath() + "/" + LOS_filePath.getBaseFileName() + LosCommon::LINUX_EXE;
#endif
    initConnect();
}

/**
析构
强行停止
*/
LosSingleCppRunner::~LosSingleCppRunner()
{
    stop();
}

/**
停止
*/
void LosSingleCppRunner::stop()
{
    if (L_gxxPro->state() != QProcess::NotRunning)
    {
        L_gxxPro->kill();
    }
    if (L_runPro->state() != QProcess::NotRunning)
    {
        L_runPro->kill();
    }
}

/*
运行
*/
void LosSingleCppRunner::start()
{
    if (!LOS_filePath.isExist())
    {
        ERR("The file to be executed does not exist...", "LosSingleCppRunner");
        return;
    }

    if (!LOS_filePath.isFile())
    {
        ERR("unknown content", "LosSingleCppRunner");
        return;
    }

    L_gxxPro->setWorkingDirectory(LOS_filePath.getAbsolutePath());
    QStringList args;
    args << LOS_filePath << LosCommon::CXX_17 << LosCommon::CMD_OBJECT << L_exePath;
    L_gxxPro->start(LosCommon::GXX, args);
}

/**
- 建立联系
*/
void LosSingleCppRunner::initConnect()
{
    connect(L_gxxPro, &QProcess::readyReadStandardError, this,
            [=]() { INF(QString::fromLocal8Bit(L_gxxPro->readAllStandardError()), "LosSingleCppRunner"); });

    connect(L_gxxPro, &QProcess::finished, this,
            [=](int exit_code, QProcess::ExitStatus exitStatus)
            {
                if (exit_code == 0)
                {
                    SUC("editing successful!", "LosSingleCppRunner");
                    L_runPro->setWorkingDirectory(LOS_filePath.getAbsolutePath());
                    L_runPro->start(L_exePath);
                }
                else
                {
                    ERR("compilation failed...", "LosSingleCppRunner");
                }
            });
            
    connect(L_gxxPro, &QProcess::errorOccurred, this,
            [=](QProcess::ProcessError error)
            {
                ERR("The compiler failed to start (possibly due to "
                    u8"g++ not being installed or an invalid path)",
                    "LosSingleCppRunner");
            });

    connect(L_runPro, &QProcess::readyReadStandardOutput, this,
            [=]() {
                INF(QString("result:\n%1").arg(QString::fromLocal8Bit(L_runPro->readAllStandardOutput())),
                    "LosSingleCppRunner");
            });

    connect(L_runPro, &QProcess::readyReadStandardError, this,
            [=]() { ERR(QString::fromLocal8Bit(L_runPro->readAllStandardError()), "LosSingleCppRunner"); });

    connect(L_runPro, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus)
            { INF("\n process terminated (exit code: " + QString::number(exitCode) + ")", "LosSingleCppRunner"); });

    connect(L_runPro, &QProcess::errorOccurred, this,
            [=](QProcess::ProcessError error) { ERR("The compiled program failed to start", "LosSingleCppRunner"); });
}

} // namespace LosCore