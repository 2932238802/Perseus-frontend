#include "LosLspManager.h"



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



    /*
     * 监听 文件结构 改变
     */
    void LosLspManager::didChangeWatchedFiles(const QString &file_path, int type)
    {
        if (file_path.contains("compile_commands.json"))
        {
            if (auto client = LOS_clients.value(LosCommon::LosToolChain_Constants::LosTool::CLANGD, nullptr))
            {
                client->didChangeWatchedFiles(
                    file_path,
                    static_cast<LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type>(type));
            }
        }
    }



    /*
     * 发送 语法 高亮请求
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
        case LosCommon::LosToolChain_Constants::LosLanguage::CMAKE:
        {
            return "cmake";
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
        {
            return "rust";
        }
        default:
            return "plaintext";
        }
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



    /*
     * onLspReady
     * - _cmd_lspReady
     * - 这个信号进行触发
     * - onCheckSingleTool 这个函数会触发
     */
    void LosLspManager::onLspReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                                   const QStringList &asgs)
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
        if (oldClient)
        {
            oldClient->didChangeWatchedFiles(
                oldPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::DELETE);
        }
        if (newClient)
        {
            newClient->didChangeWatchedFiles(
                newPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Created);
        }
    }


} /* namespace LosCore */
