// Copyright (c) 2026 LosAngelous (shengjie.lin)



#include "LosSesssion.h"
#include "core/LosRouter/LosRouter.h"
#include <qjsonobject.h>


namespace LosCore
{
    /**
     * @brief 单例模式
     *
     * @return LosSession&
     */
    LosSession &LosSession::instance()
    {
        static LosSession s;
        return s;
    }



    /**
     * @brief getDefaultConfigAbsoluteFilePath 获取 本地的session.json
     *
     * @return QString
     */
    QString LosSession::getDefaultConfigAbsoluteFilePath()
    {
        QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QDir().mkpath(configDir);
        return configDir + QDir::separator() + "session.json";
    }



    /**
     * @brief
     *
     * @param conf
     * @return true
     * @return false
     */
    bool LosSession::loadConfig(LosCommon::LosSession_Constants::Config *conf)
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
        conf->L_curActiveFile = root["activeFile"].toString();
        const QString themeName = root["themeName"].toString();
        if (!themeName.isEmpty())
        {
            conf->L_themeName = themeName;
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
        QJsonObject authConfig = root["authConfig"].toObject();
        if(!authConfig.isEmpty())
        {
            conf->LOS_authConfig.L_token = authConfig["token"].toString();
            conf->LOS_authConfig.L_username = authConfig["username"].toString();
        }
        return true;
    }



    /*
     * - 保存文件
     */
    bool LosSession::saveConfig(const LosCommon::LosSession_Constants::Config &conf)
    {
        QJsonObject obj;
        obj["projectPath"] = conf.L_curProDir;
        obj["activeFile"]  = conf.L_curActiveFile;
        QJsonArray filesList;
        for (const auto &str : conf.L_curFilePaths)
        {
            filesList.append(str);
        }
        obj["openFiles"] = filesList;
        obj["themeName"] = conf.L_themeName;
        obj["version"]   = 1;
        QJsonObject authConfig;
        authConfig["username"] = conf.LOS_authConfig.L_username;
        authConfig["token"]    = conf.LOS_authConfig.L_token;
        obj["authConfig"] = authConfig;
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

} /* namespace LosCore */