// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosLspClass.h"
#include "common/constants/ConstantsStr/LosLspStr.h"
#include "common/util/CheckLang.h"
#include "common/util/DebugPJson.h"
#include "common/util/GetLangId.h"
#include "core/LosLog/LosLog.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QProcess>
#include <QRegularExpression>
#include <QUrl>
#include <atomic>
#include <qset.h>
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

      public: // progress
        virtual void start(const QStringList &start_up_args, const QString &exe_path) = 0;
        virtual void dealLspMessage(const QJsonObject &obj)                           = 0;
        void didOpen(const QString &file_path, const QString &text, const QString &languageId);
        void didClose(const QString &file_path);
        void didChange(const QString &file_path, const QString &text);
        void requestCompletion(const QString &file_path, int line, int character);
        void requestDefinition(const QString &file_path, int line, int character);
        void requestHover(const QString &file_path, int line, int col);
        void requestSemantic(const QString &);
        void didChangeWatchedFiles(const QString &filePath, LosCommon::LosLsp_Constants::LspJson_didChangeWatchedFiles_changes_type type);

      public:
        virtual void handleFileRenamed(const QString &oldPath, const QString &newPath);

      protected:
        void sendRequest(const QString &method, const QJsonObject &params, LosLspType type);
        void sendNotification(const QString &method, const QJsonObject &params);

      private slots:
        void processRawData();

      protected:
        std::atomic<int> L_id                                         = 1;
        bool L_isinit                                                 = false;
        QProcess *L_process                                           = nullptr;
        QList<LosCommon::LosLsp_Constants::PendingRequest> L_pendings = {};
        QByteArray L_rawData                                          = {};
        QHash<QString, int> L_fileVersions                            = {};
        QMap<int, LosLspType> L_idToType                              = {};
        QSet<QString> L_openedFiles                                   = {};
        QMap<int, QString> L_idToAbsoluteFilePath                     = {};
    };

} /* namespace LosCore */
