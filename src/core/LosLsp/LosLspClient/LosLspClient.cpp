

#include "core/lsp/LosLspClient/LosLspClient.h"
#include "common/constants/ConstantsClass.h"
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qprocess.h>
#include <qregularexpression.h>
#include <qsharedpointer.h>
#include <qurl.h>

namespace LosCore {
/**
- 构造
*/
LosLspClient::LosLspClient(QObject *parent) : QObject(parent) {
  // 补充
  L_process = new QProcess(this);
  initConnect();
}

/**
- 析构
*/
LosLspClient::~LosLspClient() {
  if (L_process->state() != QProcess::NotRunning) {
    L_process->kill();
    L_process->waitForFinished(100);
  }
}

/**
- 开始
*/
void LosLspClient::start() {
  if (L_process) {
    INF("lsp start...", "LosLspClient");
    L_process->start("clangd", QStringList());
  }
}

/**
- 发送请求
*/
void LosLspClient::sendRequest(const QString &method, const QJsonObject &params,
                               LosLspType type) {
  QJsonObject request;
  request["jsonrpc"] = "2.0";
  int id = L_id.load();
  L_id++;
  L_idToType.insert(id, type);
  request["id"] = id;
  request["method"] = method;
  request["params"] = params;
  QJsonDocument doc(request);
  // 必须是紧凑才行
  QByteArray jb = doc.toJson(QJsonDocument::Compact);
  QByteArray header = LosCommon::CONTENT_LENGTH +
                      QByteArray::number(jb.size()) + LosCommon::LSP_RNRN;
  L_process->write(header + jb);
}

/**
- 发送 请求补全的请求
- 带 id 的
*/
void LosLspClient::sendRequestForCompletion(const QString &file_path, int line,
                                            int character) {
  QJsonObject textDocument;
  QJsonObject position;

  textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
  position["line"] = line;
  position["character"] = character;
  QJsonObject params;
  params["textDocument"] = textDocument;
  params["position"] = position;
  sendRequest("textDocument/completion", params, LosLspType::REQ_COMPLETION);
}

/**
- 发送不带 id 的信息
*/
void LosLspClient::sendMsg(const QString &method, const QJsonObject &param) {
  QJsonObject req;
  req["jsonrpc"] = "2.0";
  req["method"] = method;
  req["params"] = param;
  QJsonDocument doc(req);
  QByteArray jb = doc.toJson(QJsonDocument::Compact);
  QByteArray header = LosCommon::CONTENT_LENGTH +
                      QByteArray::number(jb.size()) + LosCommon::LSP_RNRN;
  L_process->write(header + jb);
  INF("[lsp]: " + method, "LosLspClient");
}

/**
文件打开
*/
void LosLspClient::didOpen(const QString &file_path, const QString &text) {
  QJsonObject textDocument;
  textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
  textDocument["languageId"] = "cpp";
  textDocument["version"] = 1;
  textDocument["text"] = text;
  QJsonObject params;
  params["textDocument"] = textDocument;
  sendMsg("textDocument/didOpen", params);
}

/**
文件修改
*/
void LosLspClient::didChange(const QString &file_path, const QString &text) {

  QJsonArray contentChanges;
  QJsonObject contentChangesObject;
  contentChangesObject["text"] = text;
  contentChanges.append(contentChangesObject);
  QJsonObject textDocument;
  textDocument["uri"] = QUrl::fromLocalFile(file_path).toString();
  textDocument["version"] = L_versionId.load();
  L_versionId++;
  QJsonObject params;
  params["textDocument"] = textDocument;
  params["contentChanges"] = contentChanges;
  sendMsg("textDocument/didChange", params);
}

/**
定位 定位
请求：
{
  "jsonrpc": "2.0",
  "id": 101,
  "method": "textDocument/definition",
  "params": {
    "textDocument": {
      "uri": "file:///home/losangelous/LosAngelous/Pro/Perseus/main.cpp"
    },
    "position": {
      "line": 15,
      "character": 12
    }
  }
}

答复:
{
  "jsonrpc": "2.0",
  "id": 101,
  "result": [
    {
      "uri": "file:///home/losangelous/LosAngelous/Pro/Perseus/utils.h",
      "range": {
        "start": {
          "line": 42,
          "character": 4
        },
        "end": {
          "line": 42,
          "character": 18
        }
      }
    }
  ]
}

*/
void LosLspClient::defineRequest(int line, int col, const QString &file_path) {
  QJsonObject params;
  QJsonObject textDocument;
  textDocument["uri"] = QUrl::fromLocalFile(file_path).toString(); ;
  QJsonObject position;
  position["line"] = line;
  position["character"] = col;
  params["textDocument"] = textDocument;
  params["position"] = position;
  sendRequest("textDocument/definition", params, LosLspType::REQ_DIFINE);
}

/**
初始化链接
*/
void LosLspClient::initConnect() {
  connect(L_process, &QProcess::started, this,
          [=]() { sendInitializeRequest(); });

  connect(L_process, &QProcess::readyReadStandardError, this, [=]() {
    auto data = L_process->readAllStandardError();
    qDebug() << "[lsp]" << QString::fromUtf8(data);
  });

  connect(L_process, &QProcess::readyReadStandardOutput, this, [=]() {
    QByteArray rawData = L_process->readAllStandardOutput();
    processRawData(rawData);
  });
}

/**
发送 初始化 请求
*/
void LosLspClient::sendInitializeRequest() {
  QJsonObject params;
  params["processId"] = QCoreApplication::applicationPid();
  QString rootPath = QDir::currentPath();
  // params["rootUri"] = "file://" + rootPath;
  // 下面更好的适应 windows
  params["rootUri"] = QUrl::fromLocalFile(rootPath).toString();
  params["capabilities"] = QJsonObject();
  sendRequest("initialize", params, LosLspType::REQ_INITIALIZE);
}

void LosLspClient::sendInitializedMsg() {
  sendMsg("initialized", QJsonObject());
}

/**
- 处理原始数据
*/
void LosLspClient::processRawData(const QByteArray &data) {

  L_rawData.append(data);

  while (true) {
    auto pos = L_rawData.indexOf("\r\n\r\n");
    if (-1 == pos)
      break;
    int headerLenth = pos;
    QByteArray headQba = L_rawData.left(headerLenth);
    QString headStr = QString::fromUtf8(headQba);

    int contentLenth = 0;
    QRegularExpression regex(R"(Content-Length:\s*(\d+))");
    QRegularExpressionMatch match = regex.match(headStr);
    if (match.hasMatch()) {
      contentLenth = match.captured(1).toInt();
    } else {
      qDebug() << "[lsp] LSP parsing error: cannot find the length "
                  "corresponding to Content-Length ";
      L_rawData.clear();
      break;
    }

    int allLenth = headerLenth + 4 + contentLenth;
    if (L_rawData.size() < allLenth) {
      qDebug() << "The content in the buffer is insufficient for a complete "
                  "LSP response; waiting for the next ReadyRead";
      break;
    }

    // 真正需要 解析的部分
    QByteArray rawJsonData = L_rawData.mid(headerLenth + 4, contentLenth);
    // 截取掉 发出去的部分
    L_rawData.remove(0, allLenth);
    QJsonDocument doc = QJsonDocument::fromJson(rawJsonData);
    if (doc.isObject()) {
      dealLspMessage(doc.object());
    } else {
      qDebug() << "[lsp] Parsing error: not a valid JSON format";
      break;
    }
  }
}

/**
- 真正处理 LSP json 数据的
*/
void LosLspClient::dealLspMessage(const QJsonObject &obj) {
  if (obj.contains("id")) {
    int id = obj["id"].toInt();
    if (!L_idToType.contains(id)) {
      qDebug() << "[lsp] Received a reply with an unknown ID";
      return;
    }

    LosLspType type = L_idToType.take(id);

    switch (type) {
    case LosLspType::REQ_COMPLETION: {
      if (obj.contains("result")) {
        QJsonObject result = obj["result"].toObject();
        QJsonArray items = result["items"].toArray();

        QStringList completionWords;

        for (int i = 0; i < items.size(); i++) {
          QJsonObject item = items[i].toObject();
          completionWords.append(item["insertText"].toString());
        }

        qDebug() << "[lsp] Successfully parsed " << completionWords.size()
                 << "completion words";

        // QStringList 这个可以删除重复的
        completionWords.removeDuplicates();
        emit _completion(completionWords);
      }
      break;
    }
    case LosLspType::REQ_INITIALIZE: {
      qDebug()
          << "[lsp] handshake successful, sending initialized confirmation";
      sendInitializedMsg();
      break;
    }
    case LosLspType::REQ_HOVER: {
      break;
    }
    case LosLspType::REQ_DIFINE :{
      if(obj.contains("result") && obj["result"].isArray())
      {
        QJsonArray arr = obj["result"].toArray();
        if(arr.empty()) break;
        QJsonObject target = arr[0].toObject();
            QString targetUri = target["uri"].toString();
            QString targetFilePath = QUrl(targetUri).toLocalFile(); 
            QJsonObject range = target["range"].toObject();
            QJsonObject start = range["start"].toObject();
            int targetLine = start["line"].toInt();
            emit _definitionResult(targetFilePath, targetLine);
      }

      break;
    }
    default: {
      break;
    }
    }
  }

  if (obj.contains("method")) {
    QString method = obj["method"].toString();
    if (method == "textDocument/publishDiagnostics") {
      QJsonObject params = obj["params"].toObject();
      QString uri = params["uri"].toString();
      QString filePath = QUrl(uri).toLocalFile(); // 把 URI 转成本地文件路径
      QJsonArray diagnostics = params["diagnostics"].toArray();
      QList<LosCommon::LosDiagnostic> diagList;
      for (int i = 0; i < diagnostics.size(); i++) {
        QJsonObject diagObj = diagnostics[i].toObject();
        LosCommon::LosDiagnostic d;
        d.message = diagObj["message"].toString();
        d.ds = static_cast<LosCommon::DiagnosticSeverity>(
            diagObj["severity"].toInt());
        QJsonObject range = diagObj["range"].toObject();
        QJsonObject start = range["start"].toObject();
        QJsonObject end = range["end"].toObject();
        d.startLine = start["line"].toInt();
        d.startChar = start["character"].toInt();
        d.endLine = end["line"].toInt();
        d.endChar = end["character"].toInt();
        diagList.append(d);
      }
      emit _diagnostics(filePath, diagList);
    }
  }
}

} // namespace LosCore