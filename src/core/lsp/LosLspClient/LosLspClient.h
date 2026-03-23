

#pragma once
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <qglobal.h>
#include <qobject.h>
#include <qprocess.h>

namespace LosCore {

class LosLspClient : public QObject {
  Q_OBJECT
public: // const
  explicit LosLspClient(QObject *parent = nullptr);
  ~LosLspClient();

public: // tool
  void start();
  void sendRequest(const QString &method, const QJsonObject &params, int id);

public:
  void initConnect();
  void sendInitializeRequest();

private:
  QProcess *L_process;
};
} // namespace LosCore