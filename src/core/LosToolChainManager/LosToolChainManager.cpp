
#include "LosToolChainManager.h"
#include "common/constants/ConstantsClass.h"
#include "common/util/GetFilePath.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFilePath/LosFilePath.h"
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qsettings.h>
#include <qstandardpaths.h>

namespace LosCore
{

LosToolChainManager::LosToolChainManager(QObject *parent) : QObject{parent}
{
    initConfig();
    initConnect();
}


/**
- 检测语言
*/
void LosToolChainManager::onCheckToolchain(LosCommon::LosToolChain_Constants::LosLanguage language)
{
    QString targatExe = LOS_Compiler.value(language);

    if (targatExe.isEmpty())
    {
        WAR("this editor has not this language-tool config!", "LosToolChainManager");
        return;
    }
    SUC("check targatExe:" + targatExe, "LosToolChainManager");

    auto config = LOS_availableTools[language][targatExe];

    QString expectedDir =
        QCoreApplication::applicationDirPath() + QDir::separator() + "tool" + QDir::separator() + config.L_name;
    ;

    if (validateExecutable(expectedDir, config))
    {
        QString findExePath = L_activeToolPath[config.L_name];
        emit LosCore::LosRouter::instance()._cmd_toolChainReady(language, findExePath);
    }
    else
    {
        emit LosCore::LosRouter::instance()._cmd_toolChainMissing(config);
    }
}



void LosToolChainManager::onCheckLspTool(LosCommon::LosToolChain_Constants::LosLanguage language)
{
    QString targetLsp = LOS_LSP.value(language);
    if (targetLsp.isEmpty())
    {
        return;
    }
    auto config = LOS_availableTools[language][targetLsp];

    QString expectedDir = QCoreApplication::applicationDirPath() + "/tools/" + config.L_name;

    if (validateExecutable(expectedDir, config))
    {
        QString foundPath = L_activeToolPath[config.L_name];
        emit LosCore::LosRouter::instance()._cmd_lspReady(language, foundPath, config.L_startupArgs);
    }
    else
    {
        emit LosCore::LosRouter::instance()._cmd_toolChainMissing(config);
    }
}


/**
- 初始化 格式
*/
void LosToolChainManager::initConfig()
{
    auto configFile(
        LosCommon::GetFilePathFromUp<LosCommon::FindFileType::SYSTEM_TOOLCHAIN_CONFIG_JSON>("toolchain_config.json"));
    if (!configFile)
    {
        ERR("config err! please exit!", "LosToolChainManager");
        return;
    }

    QFile file(*configFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ERR("failed to open config file! " + file.errorString(), "LosToolChainManager");
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        ERR("JSON parse error: " + parseError.errorString(), "LosToolChainManager");
        return;
    }
    QJsonObject root = doc.object();
    if (root.contains("sharedTools") && root["sharedTools"].isObject())
    {
        QJsonObject sharedTools = root["sharedTools"].toObject();
        for (auto it = sharedTools.begin(); it != sharedTools.end(); ++it)
        {
            // "cmake", "ninja", "git"
            QString toolName    = it.key();
            QJsonObject toolObj = it.value().toObject();
            if (toolObj.contains("supportedLanguages") && toolObj["supportedLanguages"].isArray())
            {
                QJsonArray langArray = toolObj["supportedLanguages"].toArray();
                for (const QJsonValue &langVal : langArray)
                {
                    auto targetLang = stringToLanguage(langVal.toString());
                    if (targetLang != LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
                    {
                        auto config                              = parseToolNode(toolObj, toolName, targetLang);
                        LOS_availableTools[targetLang][toolName] = config;
                    }
                }
            }
        }
    }

    if (root.contains("toolchains") && root["toolchains"].isObject())
    {
        QJsonObject toolchains = root["toolchains"].toObject();
        for (auto langIt = toolchains.begin(); langIt != toolchains.end(); ++langIt)
        {
            QString langStr     = langIt.key();
            QJsonObject langObj = langIt.value().toObject();
            auto lang           = stringToLanguage(langStr);
            if (lang == LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
                continue;
            if (langObj.contains("defaults") && langObj["defaults"].isObject())
            {
                QJsonObject defaults = langObj["defaults"].toObject();
                if (defaults.contains("compiler"))
                    LOS_Compiler[lang] = defaults["compiler"].toString();
                if (defaults.contains("lsp"))
                    LOS_LSP[lang] = defaults["lsp"].toString();
                if (defaults.contains("formatter"))
                    LOS_Formatter[lang] = defaults["formatter"].toString();
                if (defaults.contains("buildTool"))
                    LOS_BuildTool[lang] = defaults["buildTool"].toString();
            }

            if (langObj.contains("tools") && langObj["tools"].isObject())
            {
                QJsonObject tools = langObj["tools"].toObject();
                for (auto toolIt = tools.begin(); toolIt != tools.end(); ++toolIt)
                {
                    QString toolName                   = toolIt.key();
                    QJsonObject toolObj                = toolIt.value().toObject();
                    LOS_availableTools[lang][toolName] = parseToolNode(toolObj, toolName, lang);
                }
            }
        }
    }
}



void LosToolChainManager::initConnect()
{
    auto &router = LosCore::LosRouter::instance();
    connect(&router, &LosRouter::_cmd_checkToolchain, this, &LosToolChainManager::onCheckToolchain);
    connect(&router, &LosRouter::_cmd_checkLspTool, this, &LosToolChainManager::onCheckLspTool);
}



bool LosToolChainManager::validateExecutable(const QString &expectedDir,
                                             const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
{
    for (const auto &exeName : config.L_exeName)
    {
        QString sysPath = QStandardPaths::findExecutable(exeName);
        if (!sysPath.isEmpty())
        {
            L_activeToolPath[config.L_name] = sysPath;
            return true;
        }
        if (!expectedDir.isEmpty())
        {
            QDir dir(expectedDir);
            QString fullPath = dir.absoluteFilePath(exeName);
            QFileInfo fileInfo(fullPath);

            if (fileInfo.exists() && fileInfo.isExecutable())
            {
                L_activeToolPath[config.L_name] = fullPath;
                return true;
            }
        }
    }
    return false;
};



LosCommon::LosToolChain_Constants::ToolChainConfig
LosToolChainManager::parseToolNode(const QJsonObject &toolObj, const QString &toolName,
                                   LosCommon::LosToolChain_Constants::LosLanguage lang)
{
    LosCommon::LosToolChain_Constants::ToolChainConfig config;
    config.LOS_language = lang;
    config.L_category   = stringToCategory(toolObj["category"].toString());
    config.L_name       = toolObj["name"].toString();

    if (toolObj.contains("executables") && toolObj["executables"].isArray())
    {
        QJsonArray exeArray = toolObj["executables"].toArray();
        for (const QJsonValue &val : exeArray)
        {
            config.L_exeName.append(val.toString());
        }
    }

    if (toolObj.contains("startupArgs") && toolObj["startupArgs"].isArray())
    {
        QJsonArray argsArray = toolObj["startupArgs"].toArray();
        for (const QJsonValue &val : argsArray)
        {
            config.L_startupArgs.append(val.toString());
        }
    }

    config.L_requireNodeJs = toolObj["requireNodeJs"].toBool(false);
    config.L_version       = toolObj["versionArgs"].toString();
    config.L_downUrl       = toolObj["downloadUrl"].toString();
    config.L_validateKey   = toolObj["validateKey"].toString();
    return config;
}



/**
从 json 到 枚举
*/
LosCommon::LosToolChain_Constants::LosLanguage LosToolChainManager::stringToLanguage(const QString &str)
{
    using namespace LosCommon::LosToolChain_Constants;
    if (str == "CXX")
        return LosLanguage::CXX;
    if (str == "RUST")
        return LosLanguage::RUST;
    if (str == "PYTHON")
        return LosLanguage::PYTHON;
    if (str == "CSHARP")
        return LosLanguage::CSHARP;
    return LosLanguage::UNKNOWN;
}



/**
- json 里面 从字符 到 类型
*/
LosCommon::LosToolChain_Constants::ToolCategory LosToolChainManager::stringToCategory(const QString &str)
{
    using namespace LosCommon::LosToolChain_Constants;
    if (str == "LSP")
        return ToolCategory::LSP;
    if (str == "Compiler")
        return ToolCategory::Compiler;
    if (str == "Formatter")
        return ToolCategory::Formatter;
    if (str == "Linter")
        return ToolCategory::Linter;
    if (str == "Debugger")
        return ToolCategory::Debugger;
    if (str == "BuildTool")
        return ToolCategory::BuildTool;
    return ToolCategory::Compiler;
}

}; // namespace LosCore