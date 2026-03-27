

#include "LosSesssion.h"
#include <qdebug.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

namespace LosCore
{
/**
- 获取单例
*/
LosSession &LosSession::instance()
{
    static LosSession s;
    return s;
}


/**
- 获取 config 文件夹
*/
QString LosSession::getDefaultConfigAbsoluteFilePath()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    return configDir + QDir::separator() + "session.json";
}



/**
- 加载 config 文件
*/
bool LosSession::loadConfig(LosCommon::LosSession_Constants::Config* conf) // 加载本地信息
{
    QString filePath = getDefaultConfigAbsoluteFilePath();

    QFile file(filePath);
    if (!file.exists())
    {
        WAR("config file not found: " + filePath, "LosSession");
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ERR("failed to open config file: " + file.errorString(), "LosSession");
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject())
    {
        ERR("invalid config file format", "LosSession");
        return false;
    }

    QJsonObject root = doc.object();

    conf->L_curProDir = root["projectPath"].toString();
    if (!conf->L_curProDir.isEmpty() && !QDir(conf->L_curProDir).exists())
    {
        WAR("project path no longer exists: " + conf->L_curProDir, "LosSession");
        conf->L_curProDir = "";
    }

    QJsonArray filesArray = root["openFiles"].toArray();
    for (const auto &fileVal : filesArray)
    {
        QString filePath = fileVal.toString();
        if (QFileInfo::exists(filePath))
        {
            conf->L_curFilePaths.append(filePath);
        }
    }

    return true;
}



/**
- 保存文件
*/
bool LosSession::saveConfig(const LosCommon::LosSession_Constants::Config &conf) // 保存本地信息
{
    QJsonObject obj;
    obj["projectPath"] = conf.L_curProDir;
    QJsonArray filesList;
    for (const auto &str : conf.L_curFilePaths)
    {
        filesList.append(str);
    }
    obj["openFiles"] = filesList;
    obj["version"]   = 1;
    QJsonDocument doc(obj);
    QFile file(getDefaultConfigAbsoluteFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ERR("open" + getDefaultConfigAbsoluteFilePath() + " err!", "LosSession");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    SUC("save config suc", "LosSession");
    return true;
}

} // namespace LosCore