

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

#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsStr.h"
#include "core/log/LosLog/LosLog.h"

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
    REQ_HOVER,
    REQ_DIFINE,
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
  void defineRequest(int line, int col, const QString &file_path);

public: // init
  void initConnect();
  void sendInitializeRequest();
  void sendInitializedMsg();

private: // tool
  void processRawData(const QByteArray &data);
  void dealLspMessage(const QJsonObject &obj);

signals: // 补全 诊断 定位位置
  void _completion(const QStringList &words);
  void _diagnostics(const QString &file_path,
                    const QList<LosCommon::LosDiagnostic> &diags);
  void _definitionResult(const QString &file_name, int line);

private:
  std::atomic<int> L_id{1};
  std::atomic<int> L_versionId{2};
  QProcess *L_process;
  QMap<int, LosLspType> L_idToType;
  QByteArray L_rawData;
};
} // namespace LosCore