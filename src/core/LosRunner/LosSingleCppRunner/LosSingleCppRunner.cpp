#include "LosSingleCppRunner.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"


namespace LosCore
{

/**
默认构造和初始化
*/
LosSingleCppRunner::LosSingleCppRunner(QObject *parent) : LosAbstractRunner{parent}
{
    L_gxxPro = new QProcess(this);
    L_runPro = new QProcess(this);
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



/**
- 设置 当前 可执行文件的位置
*/
void LosSingleCppRunner::setExePath(const QString &exe_path)
{
    L_exePath = exe_path;
}


/*
- 运行
- 先 初始化 文件的位置
- 先 这是 输出 文件的位置
- 设置可执行 文件的位置
- 然后 运行 基础的 g++ 执行 等待 执行成功 然后 再发送完成的信号
*/
void LosSingleCppRunner::start(const QString &file_path)
{
    LOS_filePath.loadFile(file_path);
    LosModel::LosFilePath dir =
        LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);
    // #ifdef Q_OS_WIN
    //     L_outPutPath = LOS_filePath.getAbsolutePath() + QDir::separator() + LOS_filePath.getBaseFileName() +
    //                    QDir::separator() + LosCommon::LosRunner_Constants::OUTPUT_BUILD + QDir::separator() +
    //                    LosCommon::LosRunner_Constants::OUTPUT_GXX + QDir::separator() +
    //                    LosCommon::LosRunner_Constants::WIN_EXE;

    // #elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    QString outDirPath = dir.getAbsoluteFilePath() + QDir::separator() + LosCommon::LosRunner_Constants::OUTPUT_BUILD +
                         QDir::separator() + LosCommon::LosRunner_Constants::OUTPUT_GXX;
    QDir().mkpath(outDirPath);
    QString outputExe =
        outDirPath + QDir::separator() + LOS_filePath.getBaseFileName() + LosCommon::LosRunner_Constants::LINUX_EXE;
    L_outPutPath = outputExe;

    // #else
    //     L_outPutPath = LOS_filePath.getAbsolutePath() + QDir::separator() + LOS_filePath.getBaseFileName() +
    //                    LosCommon::LosRunner_Constants::WIN_EXE + QDir::separator() +
    //                    LosCommon::LosRunner_Constants::OUTPUT_BUILD + QDir::separator() +
    //                    LosCommon::LosRunner_Constants::OUTPUT_GXX + QDir::separator() +
    //                    LosCommon::LosRunner_Constants::LINUX_EXE;
    // #endif

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
    args << LOS_filePath << LosCommon::LosRunner_Constants::CXX_17 << LosCommon::LosRunner_Constants::CMD_OBJECT
         << L_outPutPath;
    L_gxxPro->start(L_exePath, args);
}



/**
- 建立联系
*/
void LosSingleCppRunner::initConnect()
{
    connect(L_gxxPro, &QProcess::readyReadStandardError, this,
            [this]() { INF(QString::fromLocal8Bit(L_gxxPro->readAllStandardError()), "LosSingleCppRunner"); });

    connect(L_gxxPro, &QProcess::finished, this,
            [this](int exit_code, QProcess::ExitStatus exitStatus)
            {
                if (exit_code == 0)
                {
                    SUC("editing successful! run : " + L_outPutPath, "LosSingleCppRunner");

                    // 新的文件 生成 触发这个信号
                    emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();
                    L_runPro->setWorkingDirectory(LOS_filePath.getAbsolutePath());
                    L_runPro->start(L_outPutPath);
                }
                else
                {
                    ERR("compilation failed...", "LosSingleCppRunner");
                }
            });

    connect(L_gxxPro, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError error)
            {
                ERR("The compiler failed to start (possibly due to "
                    u8"g++ not being installed or an invalid path)",
                    "LosSingleCppRunner");
            });

    connect(L_runPro, &QProcess::readyReadStandardOutput, this,
            [this]()
            {
                QString msg{"\n==== result ===="};
                while (L_runPro->canReadLine())
                {
                    QByteArray rawData = L_runPro->readLine();
                    QString lineStr    = QString::fromLocal8Bit(rawData).trimmed();
                    if (!lineStr.isEmpty())
                    {
                        msg += QString("\n%1").arg(lineStr);
                    }
                }
                msg += "\n================\n";
                INF(msg, "LosSingleCppRunner");
            });

    connect(L_runPro, &QProcess::readyReadStandardError, this,
            [this]() { ERR(QString::fromLocal8Bit(L_runPro->readAllStandardError()), "LosSingleCppRunner"); });

    connect(L_runPro, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus)
            { INF("process terminated (exit code: " + QString::number(exitCode) + ")", "LosSingleCppRunner"); });

    connect(L_runPro, &QProcess::errorOccurred, this,
            [=](QProcess::ProcessError error) { ERR("The compiled program failed to start", "LosSingleCppRunner"); });
}

} // namespace LosCore