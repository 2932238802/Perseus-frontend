
#include "LosLspPython.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosState/LosState.h"
#include <qcoreapplication.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
namespace LosCore
{

    LosLspPython::LosLspPython(QObject *parent) {}

    void LosLspPython::start(const QStringList &start_up_args, const QString &exe_path)
    {

        if (L_process && L_process->state() == QProcess::NotRunning)
        {
            INF("LosLspPython: " + exe_path, "LosLspPython");
            L_process->start(exe_path, start_up_args);
        }
    }



    void LosLspPython::dealLspMessage(const QJsonObject &obj)
    {
        if (obj.contains("id"))
        {
            int id = obj["id"].toInt();
            if (!L_idToType.contains(id))
                return;
            LosLspType type = L_idToType.take(id);

            switch (type)
            {
            case LosLspType::REQ_INITIALIZE:
            {
                SUC("handshake successful", "LosLspPython");
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

                sendInitializeMsg(); // 发送 initialized 通知
                for (const auto &con : L_pendings)
                {
                    didOpen(con.L_filePath, con.L_content, con.L_langId);
                }
                L_pendings.clear();
                break;
            }

            case LosLspType::REQ_COMPLETION:
            {
                if (!obj.contains("result"))
                    return;
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
                    QString insertStr = a.toObject()["insertText"].toString();
                    if (insertStr.isEmpty())
                    {
                        insertStr = a.toObject()["label"].toString();
                    }
                    if (!insertStr.isEmpty())
                    {
                        res.append(insertStr);
                    }
                }
                res.removeDuplicates();
                emit LosRouter::instance()._cmd_lsp_result_completion(res);
                break;
            }

            case LosLspType::REQ_HOVER:
            {
                if (obj.contains("result") && !obj["result"].isNull())
                {
                    QJsonObject result     = obj["result"].toObject();
                    QJsonValue contentsVal = result["contents"];
                    QString hoverText      = "";

                    if (contentsVal.isObject())
                    {
                        hoverText = contentsVal.toObject()["value"].toString();
                    }
                    else if (contentsVal.isString())
                    {
                        hoverText = contentsVal.toString();
                    }
                    else if (contentsVal.isArray())
                    {
                        QJsonArray arr = contentsVal.toArray();
                        if (!arr.isEmpty() && arr.first().isObject())
                        {
                            hoverText = arr.first().toObject()["value"].toString();
                        }
                    }
                    emit LosRouter::instance()._cmd_lsp_result_hover(hoverText);
                }
                else
                {
                    emit LosRouter::instance()._cmd_lsp_result_hover("");
                }
                break;
            }

            case LosLspType::REQ_DEFINE:
            {
                if (!obj.contains("result") || obj["result"].isNull())
                    return;
                QJsonValue res = obj["result"];
                QJsonObject target;

                if (res.isArray())
                {
                    QJsonArray ar = res.toArray();
                    if (ar.empty())
                        return;
                    target = ar.first().toObject();
                }
                else if (res.isObject())
                {
                    target = res.toObject();
                }
                else
                {
                    break;
                }

                QString fileName = QUrl(target["uri"].toString()).toLocalFile();
                int targetLine   = target["range"].toObject()["start"].toObject()["line"].toInt();
                emit LosRouter::instance()._cmd_lsp_result_definition(fileName, targetLine);
                break;
            }

            case LosLspType::REQ_SEMANTIC_HIGHLIGHT:
            {
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
                emit LosRouter::instance()._cmd_lsp_request_semantic(filePath);
            }
        }
    }



    /*
     * 申请json的格式
{
    "jsonrpc": "2.0",
    "id": 1,
    "method": "initialize",
    "params": {
        "processId": 12345,
        "rootUri": "file:///home/losangelous/LosAngelous/testPython",
        "capabilities":
        {
            "textDocument":
            {
                "completion": {},
                "semanticTokens":
                {
                    "dynamicRegistration": false,
                    "requests":
                    {
                        "full": true
                    }
                }
            },
            "workspace": {}
        },
        "initializationOptions": {
            "settings": {
                "python": {
                    "pythonPath": "python3"
                }
            }
        }
    }
}
     */
    void LosLspPython::sendInitializeRequest()
    {
        QJsonObject params;
        auto &state = LosState::instance();

        params["processId"] = QCoreApplication::applicationPid();
        if (state.contain<QString>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR))
        {
            params["rootUri"] =
                QUrl::fromLocalFile(state.get<QString>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR)).toString();
        }

        QJsonObject capabilities;
        QJsonObject textDocument;

        textDocument["completion"]   = QJsonObject{};
        capabilities["textDocument"] = textDocument;
        capabilities["workspace"]    = QJsonObject{};
        params["capabilities"]       = capabilities;

        QJsonObject initializationOptions;
        QJsonObject settings;
        QJsonObject python;

        if (state.contain<QString>(LosCommon::LosState_Constants::SG_STR::PYTHON_EXE_PATH))
        {
            python["pythonPath"] = state.get<QString>(LosCommon::LosState_Constants::SG_STR::PYTHON_EXE_PATH);
        }

        settings["python"]                = python;
        initializationOptions["settings"] = settings;

        params["initializationOptions"] = initializationOptions;
        sendRequest("initialize", params, LosLspType::REQ_INITIALIZE);
    }



    /*
     * 发送初始化的 信号
     *
     */
    void LosLspPython::sendInitializeMsg()
    {
        sendNotification("initialized", QJsonObject());
    }

} // namespace LosCore
