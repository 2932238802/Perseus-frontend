#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/constants/ConstantsStr.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
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

      public: // tool
        void requestPlugin();

      private:
        explicit LosNet(QObject *parent = nullptr);
        ~LosNet() = default;

        void initConnect();
        void conn(QNetworkReply *reply, std::function<void(const QJsonDocument &)> func);
        void request(const QString &api, std::function<void(const QJsonDocument &)> func);
        void dealPluginReply(const QJsonDocument &doc);

      private:
        QNetworkAccessManager *L_net;
    };


} // namespace LosCore
