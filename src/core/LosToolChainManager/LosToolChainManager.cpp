#include "LosToolChainManager.h"
#include "common/constants/ConstantsClass.h"
#include "core/LosRouter/LosRouter.h"


namespace LosCore
{

LosToolChainManager::LosToolChainManager(QObject *parent) : QObject{parent}
{
    initConfig();
    initConnect();
}



/**
 检查 一个 语言的 工具链
*/
void LosToolChainManager::onCheckLanguageToolchain(LosCommon::LosToolChain_Constants::LosLanguage lang,
                                                   LosCommon::LosToolChain_Constants::LosTool tool)
{
    if (!LOS_languageToolMap.contains(lang))
    {
        WAR("no tools configured for this language!", "LosToolChainManager");
        return;
    }
    onCheckSingleTool(tool);
}



/**
检查 单个 sing tool
*/
void LosToolChainManager::onCheckSingleTool(LosCommon::LosToolChain_Constants::LosTool tool)
{
    if (!LOS_toolConfigs.contains(tool))
    {
        return;
    }
    auto config = LOS_toolConfigs.value(tool);

    if (validateExecutable(config))
    {
        QString foundPath = L_activeToolPath[tool];
        SUC("tool ready: " + config.L_name + " at " + foundPath, "LosToolChainManager");
        using LosTool = LosCommon::LosToolChain_Constants::LosTool;
        switch (tool)
        {
        case LosTool::CLANGD:
        {
            INF("need clangd...", "LosToolChainManager");
            emit LosCore::LosRouter::instance()._cmd_lspReady(tool, foundPath, config.L_startupArgs);
            break;
        }
        case LosTool::NEOCMAKELSP:
        {
            INF("need neocmakelsp...", "LosToolChainManager");
            emit LosCore::LosRouter::instance()._cmd_lspReady(tool, foundPath, config.L_startupArgs);
            break;
        }
        case LosTool::CMAKE:
        {
            INF("need cmake...", "LosToolChainManager");
            emit LosCore::LosRouter::instance()._cmd_buildToolReady(tool, foundPath, config.L_startupArgs);
            break;
        }
        case LosTool::NINJA:
        {
            INF("need ninja...", "LosToolChainManager");
            emit LosCore::LosRouter::instance()._cmd_buildToolReady(tool, foundPath, config.L_startupArgs);
            break;
        }
        case LosTool::G_PLUS_PLUS:
        {
            INF("need g++...", "LosToolChainManager");

            emit LosCore::LosRouter::instance()._cmd_toolChainReady(
                LosCommon::LosToolChain_Constants::LosLanguage::CXX,
                LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS, foundPath);
            break;
        }
        default:
            break;
        }
    }
    else
    {
        WAR("Tool missing: " + config.L_name, "LosToolChainManager");
        emit LosCore::LosRouter::instance()._cmd_toolChainMissing(config);
    }
}


/**
 - 读取 json
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
            QString toolNameStr       = it.key();
            auto toolEnum             = stringToTool(toolNameStr); // 字符串转枚举
            QJsonObject toolObj       = it.value().toObject();
            LOS_toolConfigs[toolEnum] = parseToolNode(toolObj, toolNameStr);
            if (toolObj.contains("supportedLanguages") && toolObj["supportedLanguages"].isArray())
            {
                QJsonArray langArray = toolObj["supportedLanguages"].toArray();
                for (const QJsonValue &langVal : langArray)
                {
                    auto targetLang = stringToLanguage(langVal.toString());
                    if (targetLang != LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
                    {
                        if (!LOS_languageToolMap[targetLang].contains(toolEnum))
                        {
                            LOS_languageToolMap[targetLang].append(toolEnum);
                        }
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
            auto lang = stringToLanguage(langIt.key());
            if (lang == LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
                continue;

            QJsonObject langObj = langIt.value().toObject();
            if (langObj.contains("tools") && langObj["tools"].isObject())
            {
                QJsonObject tools = langObj["tools"].toObject();
                for (auto toolIt = tools.begin(); toolIt != tools.end(); ++toolIt)
                {
                    QString toolNameStr       = toolIt.key();
                    auto toolEnum             = stringToTool(toolNameStr);
                    LOS_toolConfigs[toolEnum] = parseToolNode(toolIt.value().toObject(), toolNameStr);
                    if (!LOS_languageToolMap[lang].contains(toolEnum))
                    {
                        LOS_languageToolMap[lang].append(toolEnum);
                    }
                }
            }
        }
    }
}



/**
- 初始化链接
*/
void LosToolChainManager::initConnect()
{
    auto &router = LosCore::LosRouter::instance();
    connect(&router, &LosRouter::_cmd_checkLanguageToolchain, this, &LosToolChainManager::onCheckLanguageToolchain);
    connect(&router, &LosRouter::_cmd_checkSingleTool, this, &LosToolChainManager::onCheckSingleTool);
}



/**
- 判断是不是 可以运行
*/
bool LosToolChainManager::validateExecutable(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
{
    auto toolEnum = stringToTool(config.L_name);
    for (const auto &exeName : config.L_exeName)
    {
        auto pathOpt = LosCommon::FindExePath(exeName);
        if (pathOpt.has_value())
        {
            L_activeToolPath[toolEnum] = pathOpt.value();
            return true;
        }
    }
    return false;
}



/**
- 解析 一个 config 解析
*/
LosCommon::LosToolChain_Constants::ToolChainConfig LosToolChainManager::parseToolNode(const QJsonObject &toolObj,
                                                                                      const QString &toolName)
{
    LosCommon::LosToolChain_Constants::ToolChainConfig config;
    config.L_category = stringToCategory(toolObj["category"].toString());
    config.L_name     = toolName;

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
- 字符串 -> 具体枚举
*/
LosCommon::LosToolChain_Constants::LosLanguage LosToolChainManager::stringToLanguage(const QString &str)
{
    using namespace LosCommon::LosToolChain_Constants;
    QString upperStr = str.toUpper();
    if (upperStr == "CXX")
        return LosLanguage::CXX;
    if (upperStr == "LUA")
        return LosLanguage::LUA;
    if (upperStr == "RUST")
        return LosLanguage::RUST;
    if (upperStr == "JAVA")
        return LosLanguage::JAVA;
    if (upperStr == "PYTHON")
        return LosLanguage::PYTHON;
    if (upperStr == "CSHARP")
        return LosLanguage::CSHARP;
    if (upperStr == "CMAKE")
        return LosLanguage::CMAKE;
    return LosLanguage::UNKNOWN;
}



/**
- 字符串 转 具体 枚举
*/
LosCommon::LosToolChain_Constants::ToolCategory LosToolChainManager::stringToCategory(const QString &str)
{
    using namespace LosCommon::LosToolChain_Constants;
    QString upperStr = str.toUpper();
    if (upperStr == "LSP")
        return ToolCategory::LSP;
    if (upperStr == "COMPILER")
        return ToolCategory::Compiler;
    if (upperStr == "FORMATTER")
        return ToolCategory::Formatter;
    if (upperStr == "LINTER")
        return ToolCategory::Linter;
    if (upperStr == "DEBUGGER")
        return ToolCategory::Debugger;
    if (upperStr == "BUILDTOOL")
        return ToolCategory::BuildTool;
    return ToolCategory::Compiler;
}



/**
- 字符串 转 具体工具
*/
LosCommon::LosToolChain_Constants::LosTool LosToolChainManager::stringToTool(const QString &str)
{
    using namespace LosCommon::LosToolChain_Constants;
    QString s = str.toLower();
    if (s == "cmake")
        return LosTool::CMAKE;
    if (s == "ninja")
        return LosTool::NINJA;
    if (s == "git")
        return LosTool::GIT;
    if (s == "g++" || s == "g_plus_plus")
        return LosTool::G_PLUS_PLUS;
    if (s == "clangd")
        return LosTool::CLANGD;
    if (s == "clang-format" || s == "clang_format")
        return LosTool::CLANG_FORMAT;
    if (s == "neocmakelsp" || s == "neocmakelsp.exe")
        return LosTool::NEOCMAKELSP;
    return LosTool::UNKNOWN;
}

} // namespace LosCore
