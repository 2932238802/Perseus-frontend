#include "LosRustcRunner.h"

namespace LosCore
{
    /*
     * construct
     */
    LosRustcRunner::LosRustcRunner(QObject *parent) : LosAbstractRunner(parent)
    {
        L_runner = new QProcess(this);
        L_rustc  = new QProcess(this);
        initConnect(); /* 初始化 连接 */
    }
    LosRustcRunner::~LosRustcRunner()
    {
        stop();
    }



    /*
     * 暂停
     *
     */
    void LosRustcRunner::stop()
    {
        if (L_rustc->state() != QProcess::NotRunning)
        {
            L_rustc->kill();
        }
        if (L_runner->state() != QProcess::NotRunning)
        {
            L_runner->kill();
        }
    }



    /**
     * @brief start 运行
     *
     * @param file_path
     */
    void LosRustcRunner::start(const QString &file_path)
    {
        LOS_targetFilePath.loadFile(file_path);
        /*
         * 获取 项目路径
         */
        LosModel::LosFilePath dir(
            LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR));

        QString outDirPath = dir.getAbsoluteFilePath() + QDir::separator() +
                             LosCommon::LosRunner_Constants::OUTPUT_BUILD + QDir::separator() +
                             LosCommon::LosRunner_Constants::OUTPUT_RUSTC;
        QDir().mkpath(outDirPath);
        QString outputExe = outDirPath + QDir::separator() + LOS_targetFilePath.getBaseFileName() +
                            LosCommon::LosRunner_Constants::LINUX_EXE;
        L_outPutPath = outputExe;
        /*
         * 判断路径存不存在
         */
        if (!LOS_targetFilePath.isExist())
        {
            ERR("The file to be executed does not exist...", "LosRustcRunner");
            return;
        }
        if (!LOS_targetFilePath.isFile())
        {
            ERR("unknown content", "LosRustcRunner");
            return;
        }
        L_rustc->setWorkingDirectory(LOS_targetFilePath.getAbsolutePath());
        QStringList args;
        args << LOS_targetFilePath.getAbsoluteFilePath() << LosCommon::LosRunner_Constants::CMD_OBJECT << L_outPutPath;
        L_rustc->start(L_exePath.getAbsoluteFilePath(), args);
    }



    /*
     * - 设置 可执行文件的位置
     * - 这个必须 是要 先 执行的
     */
    void LosRustcRunner::setExePath(const QString &exe_path)
    {
        L_exePath.loadFile(exe_path);
    }



    /*
     * - 初始化 连接
     */
    void LosRustcRunner::initConnect()
    {
        connect(L_rustc, &QProcess::readyReadStandardError, this,
                [this]() { INF(QString::fromLocal8Bit(L_rustc->readAllStandardError()), "LosRustcRunner"); });
        connect(L_rustc, &QProcess::finished, this,
                [this](int exit_code, QProcess::ExitStatus exitStatus)
                {
                    if (exit_code == 0)
                    {
                        SUC("rustc suc!", "LosRustcRunner");

                        /*
                         * 这里 会 创建 新的项目
                         */
                        emit LosCore::LosRouter::instance()._cmd_fileSystemChanged();

                        /*
                         * 设置 工作目录  然后 运行
                         */
                        L_runner->setWorkingDirectory(LOS_targetFilePath.getAbsolutePath());
                        L_runner->start(L_outPutPath.getAbsoluteFilePath());
                    }
                    else
                    {
                        ERR("compilation failed...", "LosRustcRunner");
                    }
                });
        connect(L_rustc, &QProcess::errorOccurred, this,
                [this](QProcess::ProcessError error)
                {
                    ERR("The compiler failed to start (possibly due to "
                        u8"rustc not being installed or an invalid path)",
                        "LosRustcRunner");
                });
        connect(L_runner, &QProcess::readyReadStandardOutput, this,
                [this]()
                {
                    QByteArray rawData = L_runner->readAllStandardOutput();
                    if (!rawData.isEmpty())
                    {
                        QString outputStr = QString::fromLocal8Bit(rawData);
                        QString msg       = QString("\n==== result ====\n%1\n================").arg(outputStr);
                        INF(msg, "LosRustcRunner");
                    }
                });
        connect(L_runner, &QProcess::readyReadStandardError, this,
                [this]() { ERR(QString::fromLocal8Bit(L_runner->readAllStandardError()), "LosRustcRunner"); });

        connect(L_runner, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus exitStatus)
                { INF("process terminated (exit code: " + QString::number(exitCode) + ")", "LosRustcRunner"); });

        connect(L_runner, &QProcess::errorOccurred, this,
                [=](QProcess::ProcessError error) { ERR("The compiled program failed to start", "LosRustcRunner"); });
    }

} /* namespace LosCore */