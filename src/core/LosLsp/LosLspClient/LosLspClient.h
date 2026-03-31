#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsStr.h"
#include "common/util/DebugPJson.h"
#include "core/LosLog/LosLog.h"
#include <QByteArray>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QProcess>
#include <QRegularExpression>
#include <QUrl>
#include <atomic>
#include <utility>

namespace LosCore
{
using LosLspType = LosCommon::LosLsp_Constants::LosLspType;

class LosLspClient : public QObject
{
    Q_OBJECT
  public:
    explicit LosLspClient(QObject *parent = nullptr);
    virtual ~LosLspClient();

    virtual void start(const QStringList &start_up_args, const QString &exe_path) = 0;
    virtual void dealLspMessage(const QJsonObject &obj)                           = 0;

    void didOpen(const QString &file_path, const QString &text, const QString &languageId);
    void didChange(const QString &file_path, const QString &text);
    void requestCompletion(const QString &file_path, int line, int character);
    void requestDefinition(const QString &file_path, int line, int character);
    void requestHover(const QString &file_path, int line, int col);
    void didChangeWatchedFiles(const QString &filePath,
                               LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type type);

  protected:
    void sendRequest(const QString &method, const QJsonObject &params, LosLspType type);
    void sendNotification(const QString &method, const QJsonObject &params);

  private slots:
    void processRawData();

  protected:
    bool L_isinit = false;
    QList<LosCommon::LosLsp_Constants::PendingRequest> L_pendings;
    QProcess *L_process;
    QByteArray L_rawData;
    std::atomic<int> L_id;
    QHash<QString, int> L_fileVersions;
    QMap<int, LosLspType> L_idToType;
};

} // namespace LosCore
