// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <QNetworkReply>
#include <QObject>
#include <qglobal.h>
#include <qurl.h>


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

      public: // tool plugin
        void requestPlugin();
        void requestRegister(const QString &username, const QString &password);
        void requestLogin(const QString &username, const QString &password);
        void requestAutoLogin(const QString &token);
        void requestAgentChat(const QString &msg, const QString &provider_name, const QString &model);
        void requestAgentFetchModels(const QString &base_url, const QString &api_key);
        void requestAgentAddProvider(const QString &provider, const QString &baseUrl, const QString &apiKey, const QStringList &models);
        void requestListProviders();
        void dealPluginReply(const QByteArray &data);
        void dealRegisterReply(const QByteArray &data);
        void dealLoginReply(const QByteArray &data);
        void dealAutoLoginReply(const QByteArray &data);
        void dealAgentChatReply(const QByteArray &data);
        void dealAgentFetchModelsReply(const QByteArray &data);
        void dealAgentAddProviderReply(const QByteArray &data);
        void dealListProvidersReply(const QByteArray &data);

      public: // tool hello
        void requestInit();
        void dealInitReply(const QByteArray &data);

      public: // tool
        void downloadPlugin(const QString &url, const QString &savePath);
        void fetchPluginReadme(const QString &urlStr, std::function<void(const QString &)> func);

      private:
        explicit LosNet(QObject *parent = nullptr);
        ~LosNet() = default;

        void initConnect();
        void conn(QNetworkReply *reply, std::function<void(const QByteArray &)> func);
        void requestGet(const QString &api, std::function<void(const QByteArray &)> func);
        void requestPost(const QString &api, const QByteArray &body, std::function<void(const QByteArray &)> func);

      private:
        QNetworkAccessManager *L_net;
    };


} /* namespace LosCore */
