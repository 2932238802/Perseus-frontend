

#pragma once
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <atomic>
#include <qglobal.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qprocess.h>

#include "common/constants/ConstantsStr.h"
#include "common/constants/ConstantsClass.h"

namespace LosCore {

class LosLspClient : public QObject {
  Q_OBJECT

public: // enum
  enum LosLspType {
    // 初始化
    REQ_INITIALIZE,
    // 语法补全
    REQ_COMPLETION,
    // 语法报错
    REQ_CHECK,
    REQ_HOVER
  };

public: // const
  explicit LosLspClient(QObject *parent = nullptr);
  ~LosLspClient();

public: // tool
  void start();
  void sendRequest(const QString &method, const QJsonObject &params,
                   LosLspType type);
  void sendRequestForCompletion(const QString &file_path, int line,
                                int character);
  void sendMsg(const QString &method, const QJsonObject &param);
  void didOpen(const QString &file_path, const QString &text);
  void didChange(const QString &file_path, const QString &text);

public: // init
  void initConnect();
  void sendInitializeRequest();
  void sendInitializedMsg();

private: // tool
  void processRawData(const QByteArray &data);
  void dealLspMessage(const QJsonObject &obj);

signals:
  void _completion(const QStringList &words);
  void _diagnostics(const QString&file_path,const QList<LosCommon::LosDiagnostic>& diags);

private:
  std::atomic<int> L_id{1};
  std::atomic<int> L_versionId{2};
  QProcess *L_process;
  QMap<int, LosLspType> L_idToType;
  QByteArray L_rawData;
};
} // namespace LosCore