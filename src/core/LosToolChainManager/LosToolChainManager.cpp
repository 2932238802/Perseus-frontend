#include "LosToolChainManager.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "common/util/StrToCategory.h"
#include "common/util/StrToLanguage.h"
#include "common/util/StrToTool.h"
#include "core/LosRouter/LosRouter.h"

namespace LosCore
{
    /**
     * @brief Construct a new Los Tool Chain Manager:: Los Tool Chain Manager object
     *
     * @param parent
     */
    LosToolChainManager::LosToolChainManager(QObject *parent) : QObject{parent}
    {
        initConfig();
        initConnect();
    }



    /**
     * @brief onCheckLanguageToolchain
     * 检查一个语言的 工具链
     *
     * @param lang
     * @param tool
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
     * @brief onCheckSingleTool
     * 检查 单个 sing tool
     *
     * @param tool
     */
    void LosToolChainManager::onCheckSingleTool(LosCommon::LosToolChain_Constants::LosTool tool)
    {
        if (!LOS_toolConfigs.contains(tool))
        {
            WAR("toolConfigs can't find anymore~", "LosToolChainManager");
            return;
        }
        auto config = LOS_toolConfigs.value(tool);

        if (validateExecutable(config))
        {
            QString foundPath = L_activeToolPath[tool];
            SUC("tool ready: " + config.L_name + " at " + foundPath, "LosToolChainManager");
            using LosTool = LosCommon::LosToolChain_Constants::LosTool;
            switch (config.L_category)
            {
            case LosCommon::LosToolChain_Constants::ToolCategory::BuildTool:
                emit LosCore::LosRouter::instance()._cmd_buildToolReady(tool, foundPath, config.L_startupArgs);
                break;

            case LosCommon::LosToolChain_Constants::ToolCategory::Compiler:
                emit LosCore::LosRouter::instance()._cmd_toolChainReady(config.LOS_language, tool, foundPath);
                break;

            case LosCommon::LosToolChain_Constants::ToolCategory::LSP:
                emit LosCore::LosRouter::instance()._cmd_lspReady(tool, foundPath, config.L_startupArgs);
                break;

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
     * @brief initConfig
     * 读取 json
     */
    void LosToolChainManager::initConfig()
    {
        auto configFile(LosCommon::GetFilePathFromUp<LosCommon::FindFileType::SYSTEM_TOOLCHAIN_CONFIG_JSON>("toolchain_config.json"));
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
                auto toolEnum             = LosCommon::StrToTool(toolNameStr);
                QJsonObject toolObj       = it.value().toObject();
                LOS_toolConfigs[toolEnum] = parseToolNode(toolObj, toolNameStr);
                if (toolObj.contains("supportedLanguages") && toolObj["supportedLanguages"].isArray())
                {
                    QJsonArray langArray = toolObj["supportedLanguages"].toArray();
                    for (const QJsonValue &langVal : langArray)
                    {
                        auto targetLang = LosCommon::StrToLanguage(langVal.toString());
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
                auto lang = LosCommon::StrToLanguage(langIt.key());
                if (lang == LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
                    continue;

                QJsonObject langObj = langIt.value().toObject();

                if (langObj.contains("tools") && langObj["tools"].isObject())
                {
                    QJsonObject tools = langObj["tools"].toObject();
                    for (auto toolIt = tools.begin(); toolIt != tools.end(); ++toolIt)
                    {
                        QString toolNameStr       = toolIt.key();
                        auto toolEnum             = LosCommon::StrToTool(toolNameStr);
                        LOS_toolConfigs[toolEnum] = parseToolNode(toolIt.value().toObject(), toolNameStr, lang);
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
     * @brief initConnect
     * 初始化链接
     */
    void LosToolChainManager::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosRouter::_cmd_checkLanguageToolchain, this, &LosToolChainManager::onCheckLanguageToolchain);
        connect(&router, &LosRouter::_cmd_checkSingleTool, this, &LosToolChainManager::onCheckSingleTool);
        connect(&router, &LosRouter::_cmd_manuallySet, this,
                [this](const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
                {
                    auto tool = LosCommon::StrToTool(config.L_name);
                    if (tool != LosCommon::LosToolChain_Constants::LosTool::UNKNOWN)
                        onCheckSingleTool(tool);
                });
    }



    /**
     * @brief validateExecutable
     * 判断是不是 可以运行
     * @param config
     * @return true
     * @return false
     */
    bool LosToolChainManager::validateExecutable(const LosCommon::LosToolChain_Constants::ToolChainConfig &config)
    {
        auto toolEnum = LosCommon::StrToTool(config.L_name);
        for (const auto &exeName : config.L_exeName)
        {
            auto pathOpt = LosCommon::FindExePath(exeName);
            if (pathOpt.has_value())
            {
                QString path(pathOpt.value());
                L_activeToolPath[toolEnum] = path;
                // 设置 setting ui 的路径
                emit LosRouter::instance()._cmd_findExePathAndSetSettingUi(path, toolEnum);
                SUC("config.L_name: " + config.L_name + " find tool in " + *pathOpt, "LosToolChainManager");
                return true;
            }
        }
        return false;
    }



    /**
     * @brief parseToolNode
     * 解析 一个 toolIt config 解析
     *
     * @param toolObj
     * @param toolName
     * @return LosCommon::LosToolChain_Constants::ToolChainConfig
     */
    LosCommon::LosToolChain_Constants::ToolChainConfig LosToolChainManager::parseToolNode(const QJsonObject &toolObj, const QString &toolName,
                                                                                          LosCommon::LosToolChain_Constants::LosLanguage language)
    {
        LosCommon::LosToolChain_Constants::ToolChainConfig config;
        config.L_category   = LosCommon::StrToCategory(toolObj["category"].toString());
        config.L_name       = toolName;
        config.LOS_language = language;
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
        if (toolObj.contains("installScripts") && toolObj["installScripts"].isObject())
        {
            QJsonObject installs = toolObj["installScripts"].toObject();
            config.L_scriptWin   = installs.contains("windows") ? installs["windows"].toString() : "";
            config.L_scriptLinux = installs.contains("linux") ? installs["linux"].toString() : "";
        }
        return config;
    }

} /* namespace LosCore */
