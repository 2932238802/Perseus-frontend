// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosLspManager.h"
#include "common/util/CheckLang.h"
#include "common/util/GetLangId.h"
#include "core/LosLsp/LosLspCMake/LosLspCMake.h"
#include "core/LosLsp/LosLspClangd/LosLspClangd.h"
#include "core/LosLsp/LosLspPython/LosLspPython.h"
#include "core/LosLsp/LosLspRust/LosLspRust.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QFile>



namespace LosCore
{
    /*
     * - construct
     */
    LosLspManager::LosLspManager(QObject *parent) : QObject(parent)
    {
        initConnect();
    }
    LosLspManager::~LosLspManager() {}



    /*
     * -  打开文件 获取 languageId
     * -  发送  didopen 信号
     */
    void LosLspManager::openFile(const QString &file_path, const QString &file_context)
    {
        if (auto client = getClient(file_path))
        {
            QString langStr = LosCommon::getLangId(LosCommon::CheckLang(file_path));
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



    /**
     * @brief didChangeWatchedFiles
     * 监听 文件结构 改变
     *
     * @param file_path
     * @param type
     */
    void LosLspManager::didChangeWatchedFiles(const QString &file_path, int type)
    {
        if (file_path.contains("compile_commands.json"))
        {
            if (auto client = LOS_clients.value(LosCommon::LosToolChain_Constants::LosTool::CLANGD, nullptr))
            {
                client->didChangeWatchedFiles(file_path, static_cast<LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type>(type));
            }
        }
    }



    /**
     * @brief onSemantic
     *
     * @param file_paht
     */
    void LosLspManager::onSemantic(const QString &file_paht)
    {
        if (auto client = getClient(file_paht))
        {
            client->requestSemantic(file_paht);
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
        connect(&router, &LosRouter::_cmd_lsp_request_semantic, this, &LosLspManager::onSemantic);
        connect(&router, &LosRouter::_cmd_fileRenamed, this, &LosLspManager::onFileRenamed);
    }



    /*
     * -
     */
    LosLspClient *LosLspManager::getClient(const QString &file_path)
    {
        auto lang = LosCommon::CheckLang(file_path);
        if (lang == LosCommon::LosToolChain_Constants::LosLanguage::CXX)
        {
            if (LOS_clients.contains(LosCommon::LosToolChain_Constants::LosTool::CLANGD))
            {
                return LOS_clients.value(LosCommon::LosToolChain_Constants::LosTool::CLANGD);
            }
        }
        else if (LosModel::LosFilePath(file_path).getFileName() == "CMakeLists.txt")
        {
            if (LOS_clients.contains(LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP))
            {
                /*
                 * 有了 在 返回
                 */
                return LOS_clients.value(LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP);
            }
        }
        else if (lang == LosCommon::LosToolChain_Constants::LosLanguage::RUST)
        {
            if (LOS_clients.contains(LosCommon::LosToolChain_Constants::LosTool::RUST_ANALYZER))
            {
                return LOS_clients.value(LosCommon::LosToolChain_Constants::LosTool::RUST_ANALYZER);
            }
        }
        else if (lang == LosCommon::LosToolChain_Constants::LosLanguage::PYTHON)
        {
            if (LOS_clients.contains(LosCommon::LosToolChain_Constants::LosTool::PYRIGHT))
            {
                return LOS_clients.value(LosCommon::LosToolChain_Constants::LosTool::PYRIGHT);
            }
        }
        return nullptr;
    }



    /**
     * @brief onLspReady
     *
     * @param tool
     * @param exePath
     * @param asgs
     */
    void LosLspManager::onLspReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath, const QStringList &asgs)
    {
        if (!LOS_clients.contains(tool))
        {
            switch (tool)
            {
            case LosCommon::LosToolChain_Constants::LosTool::CLANGD:
            {
                SUC("init c++ lsp", "LosLspManager");
                LOS_clients[tool] = new LosLspClangd(this);
                break;
            }
            case LosCommon::LosToolChain_Constants::LosTool::NEOCMAKELSP:
            {
                SUC("init cmake lsp", "LosLspManager");
                LOS_clients[tool] = new LosLspCMake(this);
                break;
            }
            case LosCommon::LosToolChain_Constants::LosTool::RUST_ANALYZER:
            {
                SUC("init rust lsp", "LosLspManager");
                LOS_clients[tool] = new LosLspRust(this);
                break;
            }
            case LosCommon::LosToolChain_Constants::LosTool::PYRIGHT:
            {
                SUC("init python lsp", "LosLspManager");
                LOS_clients[tool] = new LosLspPython(this);
                break;
            }
            default:
            {
                WAR("other lsp is not set", "LosLspManager!");
                return;
            }
            }
        }
        LOS_clients[tool]->start(asgs, exePath);
    }



    /**
     * @brief onFileRenamed 重命名
     *
     * @param oldPath
     * @param newPath
     */
    void LosLspManager::onFileRenamed(const QString &oldPath, const QString &newPath)
    {
        auto oldClient = getClient(oldPath);
        auto newClient = getClient(newPath);
        if (oldClient && oldClient == newClient)
        {
            oldClient->handleFileRenamed(oldPath, newPath);
            return;
        }
        if (oldClient)
        {
            oldClient->didClose(oldPath);
            oldClient->didChangeWatchedFiles(oldPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::DELETE);
        }
        if (newClient)
        {
            newClient->didChangeWatchedFiles(newPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Created);
            QFile f(newPath);
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString content = QString::fromUtf8(f.readAll());
                f.close();
                QString langId = LosCommon::getLangId(LosCommon::CheckLang(newPath));
                newClient->didOpen(newPath, content, langId);
            }
        }
    }


} /* namespace LosCore */
