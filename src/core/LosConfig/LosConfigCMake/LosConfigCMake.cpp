// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosConfigCMake.h"
#include <QDir>
#include <QFile>

namespace LosCore
{
    static struct regis_t<LosConfigCMake> reg_cmake("CMake");
    QStringList LosConfigCMake::files{"CMakeLists.txt"};
    LosConfigCMake::LosConfigCMake(QObject *parent) : LosConfig(parent) {}
    LosConfigCMake::~LosConfigCMake()
    {
        // 有 且 在运行 就杀死
        if (L_process && L_process->state() != QProcess::NotRunning)
        {
            L_process->kill();
            L_process->waitForFinished(500);
        }
    }


    /**
     * @brief analyse 分析
     *
     * @param projectPath
     */
    void LosConfigCMake::analyse(const QString &projectPath)
    {
        if (L_process && L_process->state() != QProcess::NotRunning)
        {
            L_process->kill();
            L_process->waitForFinished(1000);
        }

        QString buildPath = QDir(projectPath).filePath(LosCommon::LosConfig_Constants::BUILD_NAME);
        makeCMakeQueryDir(buildPath);
        QProcess *pro = new QProcess(this);
        L_process     = pro;
        pro->setWorkingDirectory(projectPath);
        QStringList args;
        args << LosCommon::LosConfig_Constants::CMD_SOURCE << LosCommon::LosConfig_Constants::SRC_DIR << LosCommon::LosConfig_Constants::CMD_TARGET
             << LosCommon::LosConfig_Constants::BUILD_NAME << LosCommon::LosConfig_Constants::COMPILE_COMMANDS_OPTION;
        // cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

        connect(pro, &QProcess::readyReadStandardOutput, this,
                [pro, this]()
                {
                    QString out = pro->readAllStandardOutput();
                    INF(out, "LosConfigCMake");
                });
        connect(pro, &QProcess::readyReadStandardError, this,
                [pro, this]()
                {
                    QString out = pro->readAllStandardError();
                    ERR(out, "LosConfigCMake");
                });
        connect(pro, &QProcess::errorOccurred, this,
                [pro, this](QProcess::ProcessError err) { ERR("cmake process error: " + QString::number(err), "LosConfigCMake"); });
        connect(pro, &QProcess::finished, this,
                [this, pro, projectPath](int exit_code, QProcess::ExitStatus status)
                {
                    if (exit_code == 0 && status == QProcess::NormalExit)
                    {
                        SUC("cmake run successful", "LosConfigCMake");

                        QString jsonPath = QDir(projectPath).filePath(LosCommon::LosConfig_Constants::BUILD_NAME) + QDir::separator() +
                                           LosCommon::LosConfig_Constants::COMPILE_COMMANDS_FILE;
                        INF("the compile_commands.json: " + jsonPath, "LosConfigCMake");
                        emit LosRouter::instance()._cmd_lsp_msg_didChangeWatchedFiles(
                            jsonPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Created);
                    }
                    else
                    {
                        ERR("cmake run error!", "LosConfigCMake");
                    }
                    L_process = nullptr;
                    pro->deleteLater();
                });
        QString msg{LosCommon::LosConfig_Constants::CMAKE};
        for (const auto &a : args)
        {
            msg += " ";
            msg += a;
        }
        INF(msg, "LosConfigCMake");
        pro->start(LosCommon::LosConfig_Constants::CMAKE, args);
    }



    /**
     * @brief isInFiles
     *  在 文件内
     *
     * @param file_name
     * @return true
     * @return false
     */
    bool LosConfigCMake::isInFiles(const QString &file_name)
    {
        return LosConfigCMake::files.contains(file_name);
    }



    /*
     * absolute_path 是项目路径
     */
    bool LosConfigCMake::can(const QString &absolute_path)
    {
        LosModel::LosFilePath filePath(absolute_path + QDir::separator() + "CMakeLists.txt");
        if (filePath.isExist() && filePath.isFile())
        {
            return true;
        }
        return false;
    }


    /*
     * - 建立 cmake 查询目录
     */

    /**
     * @brief makeCMakeQueryDir
     * 建立 cmake 查询目录
     *
     * @param build_path
     */
    void LosConfigCMake::makeCMakeQueryDir(const QString &build_path)
    {
        QString queryDir = build_path + "/.cmake/api/v1/query";
        QDir().mkpath(queryDir);
        QFile queryFile(queryDir + "/codemodel-v2");
        if (queryFile.open(QIODevice::WriteOnly))
        {
            queryFile.close();
        }
    }
} /* namespace LosCore */