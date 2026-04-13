#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include <QDir>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>

namespace LosCore
{
    class LosNet : public QObject
    {
        Q_OBJECT
      public:
        static LosNet &instance()
        {
            static LosNet _instance;
            return _instance;
        }

        LosNet(const LosNet &)            = delete;
        LosNet &operator=(const LosNet &) = delete;

      public: /* tool plugin */
        void requestPlugin();
        void dealPluginReply(const QByteArray &data);

      public: /* tool hello */
        void requestInit();
        void dealInitReply(const QByteArray &data);

      public: /* tool */
        void downloadPlugin(const QString &url, const QString &savePath);
        void fetchPluginReadme(const QString &urlStr, std::function<void(const QString &)> func);


      private:
        explicit LosNet(QObject *parent = nullptr);
        ~LosNet() = default;

        void initConnect();
        void conn(QNetworkReply *reply, std::function<void(const QByteArray &)> func);
        void request(const QString &api, std::function<void(const QByteArray &)> func);

      private:
        QNetworkAccessManager *L_net;
    };


} /* namespace LosCore */
