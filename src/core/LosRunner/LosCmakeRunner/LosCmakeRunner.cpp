
#include "LosCmakeRunner.h"


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


void LosCmakeRunner::start(const QString &cur_file_path)
{
    if (L_cmakeExe.isEmpty())
    {
        return;
    }
    if (L_cmake->state() != QProcess::NotRunning || L_runner->state() != QProcess::NotRunning)
    {
        stop();
    }

    LosModel::LosFilePath path =
        LosCore::LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);

    if (!path.isExist())
    {
        ERR("project directory not set!", "LosCmakeRunner");
        return;
    }

    QString buildPath = QDir(path.getFilePath()).filePath(LosCommon::LosConfig_Constants::BUILD_NAME);
    QDir buildDir(buildPath);
    if (!buildDir.exists())
    {
        WAR("Build directory does not exist: " + buildPath, "LosCmakeRunner");
        return;
    }
    L_cmake->setWorkingDirectory(buildPath);
    QStringList args;
    args << "--build" << ".";
    L_cmake->start(L_cmakeExe, args);
    L_curBuildPath = buildPath;
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



/**
设置 exe 的位置
*/
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
    connect(L_cmake, &QProcess::finished, this, &LosCmakeRunner::onCMakeFinished);
    connect(L_runner, &QProcess::readyReadStandardError, this,
            [=]() { ERR(QString::fromLocal8Bit(L_runner->readAllStandardError()), "LosCmakeRunner"); });
    connect(L_runner, &QProcess::readyReadStandardOutput, this,
            [=]() { INF(QString::fromLocal8Bit(L_runner->readAllStandardOutput()), "LosCmakeRunner"); });
    connect(L_runner, &QProcess::finished, this,
            [=](int exitCode, QProcess::ExitStatus exitStatus)
            {
                if (exitStatus == QProcess::NormalExit)
                {
                    INF(QString("process finished with exit code %1").arg(exitCode), "LosCmakeRunner");
                }
                else
                {
                    ERR(QString("process crashed or was killed! (Exit code: %1)").arg(exitCode), "LosCmakeRunner");
                }
            });
}



/**
- 找到 cmake 的查询文件
*/
std::optional<QString> LosCmakeRunner::findLatestIndex(const QString &build_path)
{
    QString reply = build_path + "/.cmake/api/v1/reply";
    QDir dir(reply);
    if (!dir.exists())
    {
        WAR("cmake reply dir not exist!", "LosCmakeRunner");
        return std::nullopt;
    }
    QStringList filters;
    filters << "index-*.json";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);
    QFileInfoList fileList = dir.entryInfoList();
    if (fileList.isEmpty())
    {
        WAR("no index*.json!", "LosCmakeRunner");
        return std::nullopt;
    }
    QString latestPath = fileList.first().absoluteFilePath();
    SUC("find the latestPath " + latestPath, "LosCmakeRunner");
    return latestPath;
}



/**
- 找到可执行文件的位置
*/
std::optional<QString> LosCmakeRunner::findExePath(const QString &build_path, const QString &targer_name)
{
    QString suffix = "";
#ifdef Q_OS_WIN
    suffix = ".exe";
#endif
    QString fileName         = targer_name + suffix;
    QStringList possibleDirs = {"", "Debug", "Release", "Bin"};
    QDir baseDir(build_path);
    for (const QString &sub : possibleDirs)
    {
        QString fullPath = baseDir.absoluteFilePath(sub.isEmpty() ? fileName : sub + "/" + fileName);
        QFileInfo fileInfo(fullPath);
        if (fileInfo.exists() && fileInfo.isExecutable())
        {
            SUC("find the cmake exe! : " + fullPath, "LosCmakeRunner");
            return fullPath;
        }
    }
    return std::nullopt;
}



/**
- 查找对应的json 文件
- index.json -> codemodel-v2.json -> target.json
- index 这个是 入口 买次跑Cmake都会生成 带时间戳的index
- codemodel-v2 就是整个项目的结构的路径名字
*/
std::optional<QString> LosCmakeRunner::parseCMakeTarget(const QString &index, const QString &build_path)
{
    QFile indexFile(index);
    if (!indexFile.open(QIODevice::ReadOnly))
    {
        ERR("index.json open failed!", "LosCmakeRunner");
        return std::nullopt;
    }
    QJsonDocument indexDoc  = QJsonDocument::fromJson(indexFile.readAll());
    QJsonObject indexRoot   = indexDoc.object();
    QJsonObject replyObject = indexRoot["reply"].toObject();
    if (!replyObject.contains("codemodel-v2"))
    {
        ERR("no codemodel-v2 find!", "LosCmakeRunner");
        return std::nullopt;
    }
    QString codeModelFileName = replyObject["codemodel-v2"].toObject()["jsonFile"].toString();
    QString codeModelPath     = build_path + "/.cmake/api/v1/reply/" + codeModelFileName;

    QFile codeFile(codeModelPath);
    if (!codeFile.open(QIODevice::ReadOnly))
        return std::nullopt;

    QJsonDocument codeModelDoc = QJsonDocument::fromJson(codeFile.readAll());
    QJsonObject codelModelRoot = codeModelDoc.object();

    QJsonArray configs = codelModelRoot["configurations"].toArray();
    if (configs.isEmpty())
        return std::nullopt;

    QJsonArray targets = configs.first().toObject()["targets"].toArray();
    if (targets.isEmpty())
        return std::nullopt;

    for (const QJsonValue &targetVal : targets)
    {
        QString targetFileName = targetVal.toObject()["jsonFile"].toString();
        QString targetPath     = build_path + "/.cmake/api/v1/reply/" + targetFileName;
        QFile targetFile(targetPath);
        if (!targetFile.open(QIODevice::ReadOnly))
            continue;
        QJsonDocument targetDoc = QJsonDocument::fromJson(targetFile.readAll());
        QJsonObject targetRoot  = targetDoc.object();
        if (targetRoot["type"].toString() == "EXECUTABLE")
        {
            QJsonArray artifacts = targetRoot["artifacts"].toArray();
            if (!artifacts.isEmpty())
            {
                QString relativeExePath = artifacts.first().toObject()["path"].toString();
                QString absoluteExePath = build_path + "/" + relativeExePath;
                SUC("successfully parsed executable path: " + absoluteExePath, "LosCmakeRunner");
                return absoluteExePath;
            }
        }
    }
    ERR("No executable target found!", "LosCmakeRunner");
    return std::nullopt;
}



/**
- cmake 初步 结束
*/
void LosCmakeRunner::onCMakeFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit)
    {
        auto opt = findLatestIndex(L_curBuildPath);
        if (!opt)
        {
            return;
        }
        auto exeOpt = parseCMakeTarget(*opt, L_curBuildPath);
        if (exeOpt)
        {
            INF(*exeOpt, "LosCmakeRunner");
            L_runner->setWorkingDirectory(QFileInfo(*exeOpt).absolutePath());
            L_runner->start(*exeOpt);
        }
        else
        {
            ERR("fialed to parse exe from cmake file api!", "LosCmakeRunner");
        }
    }
    else
    {
        ERR("cmake err!", "LosCmakeRunner");
    }
}
} // namespace LosCore