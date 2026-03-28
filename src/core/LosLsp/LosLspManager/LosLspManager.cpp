#include "LosLspManager.h"
#include "common/constants/ConstantsClass.h"
#include "core/LosLsp/LosLspClangd/LosLspClangd.h" // 必须引入派生类才能 new
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosRouter/LosRouter.h"
#include <QFileInfo>
#include <qlist.h>

namespace LosCore
{
/**
- construct
*/
LosLspManager::LosLspManager(QObject *parent) : QObject(parent)
{
    initConnect();
}
LosLspManager::~LosLspManager() {}



/**
- start
*/
void LosLspManager::start(const QString &file_path)
{
    LosCommon::LosToolChain_Constants::LosLanguage lang = LosCommon::CheckLang(file_path);
    emit LosCore::LosRouter::instance()._cmd_checkLspTool(lang);
}


//
void LosLspManager::openFile(const QString &file_path, const QString &file_context)
{
    if (auto client = getClient(file_path))
    {
        QString langStr = getLangId(LosCommon::CheckLang(file_path));
        client->didOpen(file_path, file_context, langStr);
    }
}

void LosLspManager::changeFile(const QString &file_path, const QString &file_context)
{
    if (auto client = getClient(file_path))
    {
        client->didChange(file_path, file_context);
    }
}


void LosLspManager::requestCompletion(const QString &file_path, int line, int col)
{
    if (auto client = getClient(file_path))
    {
        client->requestCompletion(file_path, line, col);
    }
}

void LosLspManager::toDefineRequest(int line, int col, const QString &file_path)
{
    if (auto client = getClient(file_path))
    {
        client->requestDefinition(file_path, line, col);
    }
}

void LosLspManager::didChangeWatchedFiles(const QString &file_path, int type)
{
    if (file_path.contains("compile_commands.json"))
    {
        if (auto client = LOS_clients.value(LosCommon::LosToolChain_Constants::LosLanguage::CXX, nullptr))
        {
            client->didChangeWatchedFiles(
                file_path, static_cast<LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type>(type));
        }
    }
}


void LosLspManager::initConnect()
{
    auto &router = LosRouter::instance();
    connect(&router, &LosRouter::_cmd_lsp_request_openFile, this, &LosLspManager::openFile);
    connect(&router, &LosRouter::_cmd_lsp_request_textChanged, this, &LosLspManager::changeFile);
    connect(&router, &LosRouter::_cmd_lsp_request_completeion, this, &LosLspManager::requestCompletion);
    connect(&router, &LosRouter::_cmd_whereDefine, this, &LosLspManager::toDefineRequest);
    connect(&router, &LosRouter::_cmd_lsp_msg_didChangeWatchedFiles, this, &LosLspManager::didChangeWatchedFiles);
    connect(&router, &LosRouter::_cmd_lspReady, this, &LosLspManager::onLspReady);
}



QString LosLspManager::getLangId(LosCommon::LosToolChain_Constants::LosLanguage lang)
{
    switch (lang)
    {
    case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
    {
        return "cpp";
    }
    case LosCommon::LosToolChain_Constants::LosLanguage::PYTHON:
    {
        return "python";
    }
    default:
        return "plaintext";
    }
}

LosLspClient *LosLspManager::getClient(const QString &file_path)
{
    auto lang = LosCommon::CheckLang(file_path);
    if (LOS_clients.contains(lang))
    {
        return LOS_clients[lang];
    }
    return nullptr;
}


void LosLspManager::onLspReady(LosCommon::LosToolChain_Constants::LosLanguage lan, const QString &exePath,
                               const QStringList &asgs)
{
    if (!LOS_clients.contains(lan))
    {
        switch (lan)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            LOS_clients[lan] = new LosLspClangd(this);
            break;
        }
        default:
        {
            WAR("other lsp is not set", "LosLspManager!");
            break;
        }
        }
    }
    LOS_clients[lan]->start(asgs, exePath);
}


} // namespace LosCore
