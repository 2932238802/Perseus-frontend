
#include "LosConfigCMake.h"
#include "common/constants/ConstantsStr.h"

namespace LosCore
{

static struct regis_t<LosConfigCMake> reg_cmake("CMake");
QStringList LosConfigCMake::files{"CMakeLists.txt"};
LosConfigCMake::LosConfigCMake(QObject *parent) : LosConfig(parent) {}


/**
- 分析
*/
void LosConfigCMake::analyse(const QString &projectPath)
{

    QString buildPath = QDir(projectPath).filePath(LosCommon::LosConfig_Constants::BUILD_NAME);
    makeCMakeQueryDir(buildPath);
    QProcess *pro = new QProcess(this);
    pro->setWorkingDirectory(projectPath);
    QStringList args;
    args << LosCommon::LosConfig_Constants::CMD_SOURCE << LosCommon::LosConfig_Constants::SRC_DIR
         << LosCommon::LosConfig_Constants::CMD_TARGET << LosCommon::LosConfig_Constants::BUILD_NAME
         << LosCommon::LosConfig_Constants::COMPILE_COMMANDS_OPTION;
    // cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

    connect(pro, &QProcess::readyReadStandardOutput, this,
            [=]()
            {
                QString out = pro->readAllStandardOutput();
                INF(out, "LosConfigCMake");
            });
    connect(pro, &QProcess::readyReadStandardError, this,
            [=]()
            {
                QString out = pro->readAllStandardError();
                ERR(out, "LosConfigCMake");
            });

    connect(pro, &QProcess::finished, this,
            [=](int exit_code, QProcess::ExitStatus status)
            {
                if (exit_code == 0 && status == QProcess::NormalExit)
                {
                    SUC("cmake run successful", "LosConfigCMake");

                    QString jsonPath = QDir(projectPath).filePath(LosCommon::LosConfig_Constants::BUILD_NAME) +
                                       QDir::separator() + LosCommon::LosConfig_Constants::COMPILE_COMMANDS_FILE;
                    INF("the compile_commands.json: " + jsonPath, "LosConfigCMake");
                    emit LosRouter::instance()._cmd_lsp_msg_didChangeWatchedFiles(
                        jsonPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Created);
                }
                else
                {
                    ERR("cmake run error!", "LosConfigCMake");
                }
                pro->deleteLater();
            });
    QString msg{LosCommon::LosConfig_Constants::CMAKE};
    for (auto a : args)
    {
        msg += " ";
        msg += a;
    }
    INF(msg, "LosConfigCMake");
    pro->start(LosCommon::LosConfig_Constants::CMAKE, args);
}


/**
在 文件内
*/
bool LosConfigCMake::isInFiles(const QString &file_name)
{
    return LosConfigCMake::files.contains(file_name);
}



/**
absolute_path 是项目路径
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


/**
- 建立 cmake 查询目录
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
} // namespace LosCore