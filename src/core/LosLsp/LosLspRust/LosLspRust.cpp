
#include "LosLspRust.h"

namespace LosCore
{

    LosLspRust::LosLspRust(QObject *parent) : LosLspClient(parent)
    {
        initConnect();
    };



    /*
     * - 开始
     */
    void LosLspRust::start(const QStringList &start_up_args, const QString &exe_path)
    {
        if (L_process && L_process->state() == QProcess::NotRunning)
        {
            INF("server path:" + exe_path, "LosLspRust");
            L_process->start(exe_path, start_up_args);
        }
    }



    /*
     * - 处理 rust的请求
     * - initialize
     * - initialized
     * - textDocument/didOpen              文件打开
     * - textDocument/didChange            用户编辑文件
     * - textDocument/completion           请求代码补全
     * - textDocument/definition           请求跳转到符号定义位置
     * - textDocument/hover                请求鼠标悬浮显示信息
     * - textDocument/formatting           请求格式化代码
     * - textDocument/publishDiagnostics   服务器返回诊断消息（如编译错误、警告
     * - workspace/didChangeWatchedFiles   监视工作区文件变化通知
     *
     * - textDocument/completion
     * {
     * "jsonrpc": "2.0",
     * "id": 2,
     * "method": "textDocument/completion",
     * "params": {
     * "textDocument": {
     * "uri": "file:///home/user/project/src/main.rs"
     * },
     * "position": {
     * "line": 10,
     * "character": 5
     * }
     * }
     * }
     *
     * - textDocument/definition
     * {
     * "jsonrpc": "2.0",
     * "id": 3,
     * "method": "textDocument/definition",
     * "params": {
     * "textDocument": {
     * "uri": "file:///home/user/project/src/lib.rs"
     * },
     * "position": {
     * "line": 15,
     * "character": 8
     * }
     * }
     * }
     *
     * - textDocument/hover
     * {
     * "jsonrpc": "2.0",
     * "id": 4,
     * "method": "textDocument/hover",
     * "params": {
     * "textDocument": {
     * "uri": "file:///home/user/project/src/main.rs"
     * },
     * "position": {
     * "line": 6,
     * "character": 12
     * }
     * }
     * }
     *
     *
     * - 补全的答复
     * {
     * "jsonrpc": "2.0",
     * "id": 2,
     * "result": {
     * "isIncomplete": false,
     * "items": [
     * {
     * "label": "println!",
     * "kind": 3,
     * "detail": "Macro",
     * "insertText": "println!"
     * },
     * {
     * "label": "print!",
     * "kind": 3,
     * "detail": "Macro",
     * "insertText": "print!"
     * }
     * // ...更多补全项...
     * ]
     * }
     * }
     *
     * - 跳转的返回
     * {
     * "jsonrpc": "2.0",
     * "id": 3,
     * "result": {
     * "uri": "file:///home/user/project/src/lib.rs",
     * "range": {
     * "start": { "line": 5, "character": 0 },
     * "end": { "line": 5, "character": 10 }
     * }
     * }
     * }
     *
     * - 悬浮信息响应
     * {
     * "jsonrpc": "2.0",
     * "id": 4,
     * "result": {
     * "contents": {
     * "kind": "markdown",
     * "value": "`fn main()` - The entry point of a Rust program."
     * }
     * }
     * }
     *
     */
    void LosLspRust::dealLspMessage(const QJsonObject &obj)
    {
        if (obj.contains("id"))
        {
            int id = obj["id"].toInt();
            if (!L_idToType.contains(id))
            {
                return;
            }

            LosLspType type = L_idToType.take(id);

            switch (type)
            {
            case LosCore::LosLspType::REQ_INITIALIZE:
            {
                SUC("handshake successfule", "LosLspCMake");
                L_isinit = true;
                sendInitializedMsg();
                for (const auto &con : L_pendings)
                {
                    didOpen(con.L_filePath, con.L_content, con.L_langId);
                }
                L_pendings.clear();
                break;
            }
            case LosCore::LosLspType::REQ_COMPLETION:
            {
                if (!obj.contains("result"))
                {
                    return;
                }

                QJsonValue resultVal = obj["result"];

                QJsonArray items;
                if (resultVal.isArray())
                {
                    items = resultVal.toArray();
                }
                else if (resultVal.isObject())
                {
                    items = resultVal.toObject()["items"].toArray();
                }

                QStringList res;
                for (const auto &a : items)
                {
                    QJsonObject item = a.toObject();

                    QString insertStr = item["insertText"].toString();
                    if (insertStr.isEmpty() && item.contains("textEdit"))
                    {
                        insertStr = item["textEdit"].toObject()["newText"].toString();
                    }
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
            case LosCore::LosLspType::REQ_HOVER:
            {
                if (!obj.contains("result") || obj["result"].isNull())
                    return;
                QJsonObject rst      = obj["result"].toObject();
                QJsonObject contents = rst["contents"].toObject();
                QString hoverText    = contents["value"].toString();
                emit LosRouter::instance()._cmd_lsp_result_hover(hoverText);
                break;
            }
            case LosCore::LosLspType::REQ_DEFINE:
            {
                if (!obj.contains("result") || obj["result"].isNull())
                    return;
                QJsonValue res = obj["result"];
                QJsonObject target;
                if (res.isArray())
                {
                    QJsonArray arr = res.toArray();
                    if (arr.empty())
                        return;
                    target = arr.first().toObject();
                }
                else if (res.isObject())
                {
                    target = res.toObject();
                }
                else
                {
                    return;
                }
                QString fileName     = QUrl(target["uri"].toString()).toLocalFile();
                QJsonObject startPos = target["range"].toObject()["start"].toObject();
                int line             = startPos["line"].toInt();
                emit LosRouter::instance()._cmd_lsp_result_definition(fileName, line);
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
                for (const QJsonValue &diagVal : diagnostics)
                {
                    QJsonObject diagObj = diagVal.toObject();
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



    /*
     * - 发送初始化请求
     */
    void LosLspRust::sendInitializeRequest()
    {
        QJsonObject params;
        params["processId"] = QCoreApplication::applicationPid();
        QString absPath     = LosCore::LosState::instance()
                              .get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR)
                              .getAbsoluteFilePath();
        params["rootUri"] = QUrl::fromLocalFile(absPath).toString();
        QJsonObject completion;
        QJsonObject completionItem;
        completionItem["snippetSupport"] = true;
        completion["completionItem"]     = completionItem;
        QJsonObject textDocument;
        textDocument["completion"] = completion;
        QJsonObject capabilities;
        capabilities["textDocument"] = textDocument;
        capabilities["workspace"]    = QJsonObject();
        params["capabilities"]       = capabilities;
        sendRequest("initialize", params, LosLspType::REQ_INITIALIZE);
    }



    /*
     * 处理初始化
     */
    void LosLspRust::sendInitializedMsg()
    {
        sendNotification("initialized", QJsonObject());
    }



    /*
     * - 初始化 连接
     */
    void LosLspRust::initConnect()
    {
        auto &router = LosRouter::instance();
        /*
         * 开始的时候 发送 初始化 信息先
         */
        connect(L_process, &QProcess::started, this, &LosLspRust::sendInitializeRequest);
        connect(L_process, &QProcess::readyReadStandardError, this,
                [this]() { INF(QString::fromUtf8(L_process->readAllStandardError()), "LosLspClangd"); });
        connect(&router, &LosRouter::_cmd_lsp_msg_didChangeWatchedFiles, this,
                [this](const QString &compile_commands_path, auto type)
                { this->didChangeWatchedFiles(compile_commands_path, type); });
        connect(&router, &LosRouter::_cmd_lsp_request_hover, this,
                [this](const QString &filePath, int line, int col) { this->requestHover(filePath, line, col); });
    }
} /* namespace LosCore */