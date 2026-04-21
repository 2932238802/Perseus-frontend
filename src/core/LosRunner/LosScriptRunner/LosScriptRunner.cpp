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



    /**
     * @brief
     * 调用可执行文件对指定文件进行编译
     *
     * @param in main_file_path 指定编译的文件
     */
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
        linuxArgs << "-c" << main_file_path;
        linuxArgs.append(L_args);
        L_runner->start("bash", linuxArgs);
#endif
    }



    /**
     * @brief
     * 停止QProcess运行
     */
    void LosScriptRunner::stop()
    {
        if (L_runner->state() != QProcess::NotRunning)
        {
            L_runner->kill();
        }
    }



    /**
     * @brief
     * - 初始化信号槽连接
     *
     * @note
     * - 初始化调用
     */
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

        connect(&router, &LosCore::LosRouter::_cmd_autoInstallTool, this, &LosScriptRunner::onAutoInstallTool);
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



    QString LosScriptRunner::getScriptsInstallDir()
    {
        auto &ins = LosState::instance();
        auto con  = LosCommon::LosState_Constants::SG_STR::SCRIPTS_INSTALL_DIR;
        if (!ins.contain<QString>(con))
        {
            auto opt = LosCommon::GetFilePathFromUp<LosCommon::FindFileType::COMMON_DIR>("resources/config/scripts/installers");
            if (!opt)
            {
                return "";
            }
            else
            {
                ins.set<QString>(con, *opt);
            }
        }
        return ins.get<QString>(LosCommon::LosState_Constants::SG_STR::SCRIPTS_INSTALL_DIR);
    }



    /**
     * @brief onAutoInstallTool
     * - 安装对应的工具
     * - 系统内置脚本方便 安装对应的环境
     *
     * @param config in 传入对应的工具信息
     * - 用于安装
     */
    void LosScriptRunner::onAutoInstallTool(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
    {
        auto plat = LosPlatform::getOs();
        QString allPath{getScriptsInstallDir()};
        allPath = plat == LosCommon::LosPlatform_Constants::OsType::WINDOWS
                      ? allPath + "/windows/" + config.L_scriptWin
                      : allPath + "/linux/" + config.L_scriptLinux;
        QFileInfo file(allPath);
        if (!file.exists())
        {
            ERR("not find :" + file.absoluteFilePath(), "LosScriptRunner");
            return;
        }
        this->L_args.clear();
        this->L_workingDir = file.absolutePath();
        this->start(plat == LosCommon::LosPlatform_Constants::OsType::WINDOWS ? "\"" + allPath + "\"" : allPath);
    }



} /* namespace LosCore */