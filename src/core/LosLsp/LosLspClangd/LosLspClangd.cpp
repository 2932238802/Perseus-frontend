#include "LosLspClangd.h"

namespace LosCore
{
    LosLspClangd::LosLspClangd(QObject *parent) : LosLspClient(parent)
    {
        initConnect();
    }



    /**
    开始
    */
    void LosLspClangd::start(const QStringList &start_up_args, const QString &exe_path)
    {
        if (L_process && L_process->state() == QProcess::NotRunning)
        {
            INF("serverPath : " + exe_path, "LosLspClangd");
            L_process->start(exe_path, start_up_args);
        }
    }



    /**
    初始化 链接
    */
    void LosLspClangd::initConnect()
    {
        auto &router = LosRouter::instance();
        // 开始的时候 发送 初始化 信息先
        connect(L_process, &QProcess::started, this, &LosLspClangd::sendInitializeRequest);
        connect(L_process, &QProcess::readyReadStandardError, this,
                [this]() { INF(QString::fromUtf8(L_process->readAllStandardError()), "LosLspClangd"); });
        connect(&router, &LosRouter::_cmd_lsp_msg_didChangeWatchedFiles, this,
                [this](const QString &compile_commands_path, auto type)
                { this->didChangeWatchedFiles(compile_commands_path, type); });
        connect(&router, &LosRouter::_cmd_lsp_request_hover, this,
                [this](const QString &filePath, int line, int col) { this->requestHover(filePath, line, col); });
    }



    /**
    - 发送初始化请求
    */
    void LosLspClangd::sendInitializeRequest()
    {
        QJsonObject params;
        params["processId"] = QCoreApplication::applicationPid();
        params["rootUri"]   = QUrl::fromLocalFile(QDir::currentPath()).toString();
        QJsonObject capabilities;
        QJsonObject semanticTokens;
        semanticTokens["dynamicRegistration"] = false;
        semanticTokens["requests"]            = QJsonObject{{"full", true}};
        capabilities["textDocument"]          = QJsonObject{{"semanticTokens", semanticTokens}};
        params["capabilities"]                = capabilities;
        sendRequest("initialize", params, LosLspType::REQ_INITIALIZE);
    }



    /**
    处理初始化
    */
    void LosLspClangd::sendInitializedMsg()
    {
        sendNotification("initialized", QJsonObject());
    }



    /**
    - 处理 lsp 请求
    */
    void LosLspClangd::dealLspMessage(const QJsonObject &obj)
    {
        if (obj.contains("id"))
        {
            int id = obj["id"].toInt();
            if (!L_idToType.contains(id))
                return;
            LosLspType type = L_idToType.take(id);
            switch (type)
            {
            case LosLspType::REQ_COMPLETION:
            {
                if (!obj.contains("result"))
                    return;
                QJsonObject result = obj["result"].toObject();
                QJsonArray items   = result["items"].toArray();
                QStringList res;
                res.clear();
                for (const auto &a : items)
                {
                    QJsonObject item  = a.toObject();
                    QString insertStr = item["insertText"].toString();
                    if (insertStr.isEmpty())
                    {
                        insertStr = item["label"].toString();
                    }
                    res.append(insertStr);
                }
                res.removeDuplicates();
                emit LosRouter::instance()._cmd_lsp_result_completion(res);
                break;
            }
            // 有 id 的请求 就 一定会返回 有id 的回复
            case LosLspType::REQ_INITIALIZE:
            {
                SUC("handshake successful", "LosLspClangd");
                L_isinit = true;
                if (obj.contains("result"))
                {
                    QJsonObject result       = obj["result"].toObject();
                    QJsonObject capabilities = result["capabilities"].toObject();
                    if (capabilities.contains("semanticTokensProvider"))
                    {
                        QJsonObject provider = capabilities["semanticTokensProvider"].toObject();
                        QJsonObject legend   = provider["legend"].toObject();

                        QStringList tokenTypes;
                        for (const QJsonValue &val : legend["tokenTypes"].toArray())
                        {
                            tokenTypes.append(val.toString());
                        }

                        QStringList tokenModifiers;
                        for (const QJsonValue &val : legend["tokenModifiers"].toArray())
                        {
                            tokenModifiers.append(val.toString());
                        }
                        emit LosRouter::instance()._cmd_lsp_result_semanticLegend(tokenTypes, tokenModifiers);
                    }
                }
                sendInitializedMsg();
                for (const auto &con : L_pendings)
                {
                    didOpen(con.L_filePath, con.L_content, con.L_langId);
                }
                L_pendings.clear();
                break;
            }
            case LosLspType::REQ_DEFINE:
            {
                if (obj.contains("result") && obj["result"].isArray())
                {
                    QJsonArray arr = obj["result"].toArray();
                    if (arr.empty())
                        break;
                    QJsonObject target     = arr[0].toObject();
                    QString targetFilePath = QUrl(target["uri"].toString()).toLocalFile();
                    int targetLine         = target["range"].toObject()["start"].toObject()["line"].toInt();
                    emit LosRouter::instance()._cmd_lsp_result_definition(targetFilePath, targetLine);
                }
                break;
            }
            case LosCore::LosLspType::REQ_HOVER:
            {
                if (obj.contains("result") && !obj["result"].isNull())
                {
                    QJsonObject result   = obj["result"].toObject();
                    QJsonObject contents = result["contents"].toObject();
                    QString hoverText    = contents["value"].toString();
                    // 悬停 提示 前端 ui 进行渲染
                    emit LosRouter::instance()._cmd_lsp_result_hover(hoverText);
                }
                else
                {
                    emit LosRouter::instance()._cmd_lsp_result_hover("");
                }
                break;
            }
            case LosCore::LosLspType::REQ_SEMANTIC_HIGHLIGHT:
            {
                // 这里是 语法 高亮
                // [deltaLine, deltaChar, length, tokenType, modifiers]
                // json 返回的 只是 数组 没有对应的字段
                // 就是 纯 数字 所以需要自己进行解析
                INF("11", "11");

                if (obj.contains("result") && !obj["result"].isNull())
                {
                    QJsonObject result = obj["result"].toObject();
                    if (result.contains("data"))
                    {
                        QJsonArray data = result["data"].toArray();
                        emit LosRouter::instance()._cmd_lsp_result_semanticTokens(data);
                    }
                }
                break;
            }
            default:
                break;
            }
        }

        if (obj.contains("method"))
        {
            QString method = obj["method"].toString();
            if (method == "textDocument/publishDiagnostics")
            {
                QJsonObject params     = obj["params"].toObject();
                QString filePath       = QUrl(params["uri"].toString()).toLocalFile();
                QJsonArray diagnostics = params["diagnostics"].toArray();

                QList<LosCommon::LosLsp_Constants::LosDiagnostic> diagList;
                for (int i = 0; i < diagnostics.size(); i++)
                {
                    QJsonObject diagObj = diagnostics[i].toObject();
                    LosCommon::LosLsp_Constants::LosDiagnostic d;
                    d.message = diagObj["message"].toString();
                    d.ds = static_cast<LosCommon::LosLsp_Constants::DiagnosticSeverity>(diagObj["severity"].toInt());

                    QJsonObject start = diagObj["range"].toObject()["start"].toObject();
                    QJsonObject end   = diagObj["range"].toObject()["end"].toObject();
                    d.startLine       = start["line"].toInt();
                    d.startChar       = start["character"].toInt();
                    d.endLine         = end["line"].toInt();
                    d.endChar         = end["character"].toInt();
                    diagList.append(d);
                }
                emit LosRouter::instance()._cmd_lsp_result_diagnostics(filePath, diagList);
            }
        }
    }

} // namespace LosCore
