#include "LosLspClient.h"

namespace LosCore
{
    /*
     * - construct
     */
    LosLspClient::LosLspClient(QObject *parent) : QObject(parent)
    {
        L_process = new QProcess(this);
        connect(L_process, &QProcess::readyReadStandardOutput, this, &LosLspClient::processRawData);
    }



    /*
     * - 析构
     */
    LosLspClient::~LosLspClient()
    {
        if (L_process && L_process->state() != QProcess::NotRunning)
        {
            L_process->kill();
            L_process->waitForFinished(100);
        }
    }



    /*
     * 打开文件
     */
    void LosLspClient::didOpen(const QString &file_path, const QString &text, const QString &languageId)
    {
        if (!L_isinit)
        {
            LosCommon::LosLsp_Constants::PendingRequest req;
            req.L_content  = std::move(text);
            req.L_filePath = std::move(file_path);
            req.L_langId   = std::move(languageId);
            L_pendings.append(req);
            return;
        }
        QJsonObject textDocument;
        textDocument["uri"]        = QUrl::fromLocalFile(file_path).toString();
        textDocument["languageId"] = languageId;
        textDocument["version"]    = 1;
        L_fileVersions[file_path]  = 1;
        textDocument["text"]       = text;
        QJsonObject params;
        params["textDocument"] = textDocument;
        sendNotification("textDocument/didOpen", params);
    }



    /**
     * @brief didClose 通知 LSP 服务器某文件已关闭
     *
     * @param file_path 关闭的文件路径
     *
     * LSP 消息格式:
     * {
     *   "jsonrpc": "2.0",
     *   "method": "textDocument/didClose",
     *   "params": {
     *     "textDocument": {
     *       "uri": "file:///path/to/file"
     *     }
     *   }
     * }
     */
    void LosLspClient::didClose(const QString &file_path)
    {
        if (!L_isinit)
            return;
        if (!file_path.isEmpty())
            return;

        QJsonObject textDocument;
        textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
        QJsonObject params;
        params["textDocument"] = textDocument;
        sendNotification("textDocument/didClose", params);
        L_openedFiles.remove(file_path);
        L_fileVersions.remove(file_path);
    }



    /**
     * @brief didChange
     *
     * @param file_path
     * @param text
     */
    void LosLspClient::didChange(const QString &file_path, const QString &text)
    {
        if (!L_isinit)
        {
            for (auto &req : L_pendings)
            {
                if (req.L_filePath == file_path)
                {
                    req.L_content = text;
                    return;
                }
            }
            return;
        }
        QJsonArray contentChanges;
        QJsonObject contentChangesObject;
        contentChangesObject["text"] = text;
        contentChanges.append(contentChangesObject);
        QJsonObject textDocument;
        int newVersion            = L_fileVersions[file_path] + 1;
        L_fileVersions[file_path] = newVersion;
        textDocument["uri"]       = QUrl::fromLocalFile(file_path).toString();
        textDocument["version"]   = newVersion;
        QJsonObject params;
        params["textDocument"]   = textDocument;
        params["contentChanges"] = contentChanges;
        sendNotification("textDocument/didChange", params);
    }



    /**
     * @brief requestCompletion
     *
     * @param in file_path
     * @param in line
     * @param in character
     */
    void LosLspClient::requestCompletion(const QString &file_path, int line, int character)
    {
        if (!L_isinit)
            return;
        QJsonObject textDocument;
        textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
        QJsonObject position;
        position["line"]      = line;
        position["character"] = character;
        QJsonObject params;
        params["textDocument"] = textDocument;
        params["position"]     = position;
        sendRequest("textDocument/completion", params, LosLspType::REQ_COMPLETION);
    }



    /**
     * @brief requestDefinition
     *
     * @param file_path
     * @param line
     * @param character
     */
    void LosLspClient::requestDefinition(const QString &file_path, int line, int character)
    {
        QJsonObject textDocument;
        textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
        QJsonObject position;
        position["line"]      = line;
        position["character"] = character;
        QJsonObject params;
        params["textDocument"] = textDocument;
        params["position"]     = position;
        sendRequest("textDocument/definition", params, LosLspType::REQ_DEFINE);
    }



    /**
     * @brief requestHover
     *
     * @param file_path
     * @param line
     * @param character
     */
    void LosLspClient::requestHover(const QString &file_path, int line, int character)
    {
        QJsonObject position;
        position["line"]      = line;
        position["character"] = character;
        QJsonObject textDocument;
        textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
        QJsonObject params;
        params["textDocument"] = textDocument;
        params["position"]     = position;
        sendRequest("textDocument/hover", params, LosLspType::REQ_HOVER);
    }



    /**
     * @brief requestSemantic
     *
     * @param file_path
     */
    void LosLspClient::requestSemantic(const QString &file_path)
    {
        if (!L_isinit)
        {
            return;
        }
        QJsonObject params;
        QJsonObject textDocument;
        textDocument["uri"]    = QUrl::fromLocalFile(file_path).toString();
        params["textDocument"] = textDocument;
        sendRequest("textDocument/semanticTokens/full", params, LosLspType::REQ_SEMANTIC_HIGHLIGHT);
    }



    /**
     * @brief didChangeWatchedFiles
     *
     * @param filePath
     * @param type
     */
    void
    LosLspClient::didChangeWatchedFiles(const QString &filePath,
                                        LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type type)
    {
        QJsonObject params, change;
        QJsonArray changes;
        change["uri"]  = QUrl::fromLocalFile(filePath).toString();
        change["type"] = static_cast<int>(type);
        changes.append(change);
        params["changes"] = changes;
        sendNotification("workspace/didChangeWatchedFiles", params);
    }



    /**
     * @brief handleFileRenamed 重命名
     *
     * @param oldPath
     * @param newPath
     */
    void LosLspClient::handleFileRenamed(const QString &oldPath, const QString &newPath)
    {
        if (!L_isinit)
            return;
        bool open = L_openedFiles.contains(oldPath);
        if (!open)
        {
            didClose(oldPath);
        };

        didChangeWatchedFiles(oldPath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::DELETE);
        didChangeWatchedFiles(oldPath,
                              LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type::Created);
        if (open)
        {
            QFile f(newPath);
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString cont = QString::fromUtf8(f.readAll());
                f.close();
                QString langStr = LosCommon::getLangId(LosCommon::CheckLang(newPath));
            }
        }
    }



    /**
     * @brief
     *
     * @param method
     * @param params
     * @param type
     */
    void LosLspClient::sendRequest(const QString &method, const QJsonObject &params, LosLspType type)
    {
        QJsonObject request;
        request["jsonrpc"] = "2.0";
        int id             = L_id.fetch_add(1);
        L_idToType.insert(id, type);
        request["id"]     = id;
        request["method"] = method;
        request["params"] = params;

        QJsonDocument doc(request);
        QByteArray jb     = doc.toJson(QJsonDocument::Compact);
        QByteArray header = LosCommon::LosLsp_Constants::CONTENT_LENGTH + QByteArray::number(jb.size()) +
                            LosCommon::LosLsp_Constants::LSP_RNRN;
        L_process->write(header + jb);
    }



    /**
     * @brief
     *
     * @param method
     * @param params
     */
    void LosLspClient::sendNotification(const QString &method, const QJsonObject &params)
    {
        QJsonObject req;
        req["jsonrpc"] = "2.0";
        req["method"]  = method;
        req["params"]  = params;
        QJsonDocument doc(req);
        QByteArray jb     = doc.toJson(QJsonDocument::Compact);
        QByteArray header = LosCommon::LosLsp_Constants::CONTENT_LENGTH + QByteArray::number(jb.size()) +
                            LosCommon::LosLsp_Constants::LSP_RNRN;
        L_process->write(header + jb);
    }



    /**
     * @brief 手动解析数据
     *
     */
    void LosLspClient::processRawData()
    {
        QByteArray data = L_process->readAllStandardOutput();
        L_rawData.append(data);
        while (true)
        {
            auto pos = L_rawData.indexOf("\r\n\r\n");
            if (-1 == pos)
                break;
            int headerLenth    = pos;
            QByteArray headQba = L_rawData.left(headerLenth);
            QString headStr    = QString::fromUtf8(headQba);

            int contentLenth = 0;
            QRegularExpression regex(R"(Content-Length:\s*(\d+))");
            QRegularExpressionMatch match = regex.match(headStr);
            if (match.hasMatch())
            {
                contentLenth = match.captured(1).toInt();
            }
            else
            {
                ERR("cannot find Content-Length", "LosLspClient");
                L_rawData.clear();
                break;
            }
            int allLenth = headerLenth + 4 + contentLenth;
            if (L_rawData.size() < allLenth)
            {
                break;
            }
            QByteArray rawJsonData = L_rawData.mid(headerLenth + 4, contentLenth);
            L_rawData.remove(0, allLenth);
            QJsonDocument doc = QJsonDocument::fromJson(rawJsonData);
            if (doc.isObject())
            {
                dealLspMessage(doc.object());
            }
            else
            {
                ERR("parse error", "LosLspClient");
                break;
            }
        }
    }



} /* namespace LosCore */
