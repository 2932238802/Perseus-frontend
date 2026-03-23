

#include "core/lsp/LosLspClient/LosLspClient.h"
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qjsonobject.h>
#include <qprocess.h>
#include <qsharedpointer.h>

namespace LosCore {
LosLspClient::LosLspClient(QObject *parent) : QObject(parent) {}
LosLspClient::~LosLspClient() {
  if (L_process->state() != QProcess::NotRunning) {
    L_process->kill();
  }
}

void LosLspClient::start() {
  if (!L_process) {
    L_process->start("clangd", QStringList());
  }
}

void LosLspClient::sendRequest(const QString &method, const QJsonObject &params,
                               int id) {
  QJsonObject request;
  request["jsonrpc"] = "2.0";
  request["id"] = id;
  request["method"] = method;
  request["params"] = params;

  QJsonDocument doc(request);
  QByteArray jb = doc.toJson();
  QByteArray header =
      "Content-Length: " + QByteArray::number(jb.size()) + "\r\n\r\n";
  L_process->write(header + jb);
}

void LosLspClient::initConnect() {
  connect(L_process, &QProcess::started, this,
          [=]() { sendInitializeRequest(); });

  connect(L_process, &QProcess::readyReadStandardError, this, [=]() {
    auto data = L_process->readAllStandardError();
    qDebug() << "[lsp]" << QString::fromUtf8(data);
  });

  connect(L_process, &QProcess::readyReadStandardOutput, this, [=]() {
    QByteArray rawData = L_process->readAllStandardOutput();
    qDebug() << "\n========== [LSP 收到回信 ] ==========";
    qDebug().noquote() << QString::fromUtf8(rawData);
    qDebug() << "=============================================\n";
  });
}

void LosLspClient::sendInitializeRequest() {
  QJsonObject params;
  params["processId"] = QCoreApplication::applicationPid();
  QString rootPath = QDir::currentPath();
  params["rootUri"] = "file://" + rootPath;
  params["capabilities"] = QJsonObject();
  sendRequest("initialize", params, 1);
}

} // namespace LosCore