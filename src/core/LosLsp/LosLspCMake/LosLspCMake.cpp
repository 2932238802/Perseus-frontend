#include "LosLspCMake.h"
#include "common/util/DebugPJson.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qprocess.h>

namespace LosCore
{

LosLspCMake::LosLspCMake(QObject *parent) : LosLspClient(parent)
{
    initConnect();
}

/**
- 开始
*/
void LosLspCMake::start(const QStringList &start_up_args, const QString &exe_path)
{
    if (L_process && L_process->state() == QProcess::NotRunning)
    {
        INF("LosLspCMake : " + exe_path, "LosLspCMake");
        L_process->start(exe_path, start_up_args);
    }
}



/**
- 处理 请求
*/
void LosLspCMake::dealLspMessage(const QJsonObject &obj)
{
    // initialized
    // 无 id 也没有响应
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
        case LosCore::LosLspType::REQ_CHECK:
        {
            break;
        }
        case LosCore::LosLspType::REQ_INITIALIZE:
        {
            SUC("handshake successful", "LosLspCMake");
            // 初始化的 回复
            L_isinit = true;
            // 发送通知
            sendInitializeMsg();
            for (const auto &con : L_pendings)
            {
                didOpen(con.L_filePath, con.L_content, con.L_langId);
            }

            L_pendings.clear();
            break;
        }
        case LosCore::LosLspType::REQ_COMPLETION:
        {
            // 补全
            // {
            //   "jsonrpc": "2.0",
            //   "id": 2,
            //   "result": {
            //     "isIncomplete": false,
            //     "items": [
            //       {
            //         "label": "project",
            //         "kind": 3,
            //         "detail": "project(<PROJECT-NAME> ...)",
            //         "insertText": "project"
            //       },
            //       {
            //         "label": "add_executable",
            //         "kind": 3,
            //         "detail": "add_executable(<name> ...)",
            //         "insertText": "add_executable"
            //       }
            //     ]
            //   }
            // }
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
            // 去掉 重复的
            res.removeDuplicates();
            emit LosRouter::instance()._cmd_lsp_result_completion(res);
            break;
        }
        case LosCore::LosLspType::REQ_HOVER:
        {
            // {
            //   "jsonrpc": "2.0",
            //   "id": 3,
            //   "result": {
            //     "contents": {
            //       "kind": "markdown",
            //       "value": "**project**\n\nSet the name, version, and languages of the
            //       project.\n\n```cmake\nproject(<PROJECT-NAME> [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
            //       [LANGUAGES <language-name>...])\n```"
            //     },
            //     "range": {
            //       "start": { "line": 5, "character": 0 },
            //       "end": { "line": 5, "character": 7 }
            //     }
            //   }
            // }
            if (obj.contains("result") && !obj["result"].isNull())
            {
                QJsonObject result   = obj["result"].toObject();
                QJsonObject contents = result["contents"].toObject();
                QString hoverText    = contents["value"].toString();
                emit LosRouter::instance()._cmd_lsp_result_hover(hoverText);
            }
            else
            {
                emit LosRouter::instance()._cmd_lsp_result_hover("");
            }
            break;
        }
        case LosCommon::LosLsp_Constants::LosLspType::REQ_DEFINE:
        {
            // {
            //   "jsonrpc": "2.0",
            //   "id": 4,
            //   "result": {
            //     "uri": "file:///usr/share/cmake-3.28/Modules/FindQt6.cmake",
            //     "range": {
            //       "start": { "line": 1, "character": 0 },
            //       "end": { "line": 1, "character": 20 }
            //     }
            //   }
            // }
            if (!obj.contains("result") || obj["result"].isNull())
            {
                return;
            }
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
            QString fileName     = QUrl(target["uri"].toString()).toLocalFile();
            QJsonObject startPos = target["range"].toObject()["start"].toObject();
            int targetLine       = startPos["line"].toInt();
            int targetChar       = startPos["character"].toInt(); // 这个
            emit LosRouter::instance()._cmd_lsp_result_definition(fileName, targetLine);
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
                d.ds      = static_cast<LosCommon::LosLsp_Constants::DiagnosticSeverity>(diagObj["severity"].toInt());
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



/**
- 初始化 连接
*/
void LosLspCMake::initConnect()
{
    // 开始的时候 发送 初始化 信息
    connect(L_process, &QProcess::started, this, &LosLspCMake::sendInitializeRequest);
    connect(L_process, &QProcess::readyReadStandardError, this,
            [=]() { INF(QString::fromUtf8(L_process->readAllStandardError()), "LosLspCMake"); });
    connect(L_process, &QProcess::errorOccurred, this,
            [=](QProcess::ProcessError err) { INF("QProcess error: " + QString::number(err), "LosLspCMake"); });
    connect(L_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [=](int code, QProcess::ExitStatus status)
            { INF("QProcess finished, code: " + QString::number(code), "LosLspCMake"); });
    auto &router = LosCore::LosRouter::instance();
    connect(&router, &LosRouter::_cmd_lsp_request_hover, this,
            [=](const QString &filePath, int line, int col) { this->requestHover(filePath, line, col); });
}



/**
{
  "jsonrpc": "2.0",
  "id": 1,
  "method": "initialize",
  "params": {
    "processId": 12345,
    "rootPath": "/path/to/your/project",
    "rootUri": "file:///path/to/your/project",
    "capabilities": {
      "textDocument": {
        "completion": {
          "dynamicRegistration": false
        }
      }
    }
  }
}
- 请求体的方式
- rootPath  被官方 废弃
- neocmakelsp 的核心补全功能，对客户端能力无强依赖，空对象完全够用
- 所以 capabilities 为空
*/
void LosLspCMake::sendInitializeRequest()
{
    QJsonObject params;
    params["processId"] = QCoreApplication::applicationPid();
    QString absPath     = LosCore::LosState::instance()
                          .get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR)
                          .getAbsoluteFilePath();
    params["rootUri"] = QUrl::fromLocalFile(absPath).toString();
    QJsonObject completion;
    QJsonObject textDocument;
    textDocument["completion"] = completion;
    QJsonObject capabilities;
    capabilities["textDocument"] = textDocument;
    capabilities["workspace"]    = QJsonObject();
    params["capabilities"]       = capabilities;
    sendRequest("initialize", params, LosLspType::REQ_INITIALIZE);
}



/**
- 发送 初始化 通知
- 1 客户端 —> 服务端 initialize
- 2 服务端 -> 客户端 initialize
- 3 客户端 → 服务端	Notification (不带 id)
*/
void LosLspCMake::sendInitializeMsg()
{
    sendNotification("initialized", QJsonObject());
}
} // namespace LosCore