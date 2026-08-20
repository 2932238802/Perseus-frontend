// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <qtmetamacros.h>

class QTimer;
class QWebChannel;
class QWebEngineView;

namespace LosView
{
    class LosAgentUi;

    class LosAgentBridge : public QObject
    {
        Q_OBJECT
      public:
        explicit LosAgentBridge(LosAgentUi *ui, QObject *parent = nullptr);
        Q_INVOKABLE void sendMessage(const QString &text);
        Q_INVOKABLE void providerChanged(const QString &name);
        Q_INVOKABLE void addProvider();
        Q_INVOKABLE void refreshProviders();
        Q_INVOKABLE void deleteProvider();
        Q_INVOKABLE void deleteModel();
        Q_INVOKABLE void initState();
      private:
        LosAgentUi *L_ui;
    };

    class LosAgentUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosAgentUi(QWidget *parent = nullptr);
        ~LosAgentUi();

        void onUserSend(const QString &text);
        void onAddClicked();
        void onDeleteProvider();
        void onDeleteModel();
        void onProviderChanged(const QString &name);
        void onRefreshProviders();
        void onInitState();

      private slots:
        void onProviderChanged(int index);
        void onProvidersReceived(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg);
        void onProviderAdded(bool success, const QString &message);
        void onProviderDeleted(bool ok, const QString &message);
        void onAgentReply(const QString &message);
        void onAgentError(const QString &message);
        void onReplyChunk(const QString &data);
        void onReplyDone();
        void flushChunks();

      private:
        void initUi();
        void initConnect();
        void loadProviders();
        void setProviderModels(const QStringList &providers, const QStringList &models);
        void applyThemeToWeb();
        void runJs(const QString &js);

      private:
        QWebEngineView *L_webView;
        QWebChannel *L_channel;
        QObject *L_bridge;

        QMap<QString, QStringList> L_providerModels;
        QString L_currentProvider;
        QString L_currentModel;
        QString L_streamingBuffer;
        QString L_pendingChunk;
        QTimer *L_flushTimer = nullptr;
        bool L_pageReady = false;
    };
} /* namespace LosView */