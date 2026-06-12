// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "core/LosRouter/LosRouter.h"

#include <QMap>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <qtmetamacros.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LosAgentUi;
}
QT_END_NAMESPACE

namespace LosView
{
    class LosAgentUi : public QWidget
    {
        Q_OBJECT
      public: // construct
        explicit LosAgentUi(QWidget *parent = nullptr);
        ~LosAgentUi();

      private: 
        enum class Role
        {
            User,
            Agent
        };
      private slots: // chs
        void onSendClicked();
        void onAddClicked();
        void onProviderChanged(int index);
        void onProvidersReceived(bool ok, const QMap<QString, QStringList> &providerModels, const QString &msg);
        void onProviderAdded(bool success, const QString &message);
        void onAgentReply(const QString &message);
        void onAgentError(const QString &message);
        void applyTheme(const QString &themeName);

      private: // init
        void initUi();
        void initStyle();
        void initConnect();

      private: // tool
        void addBubble(Role role, const QString &content);
        void loadProviders();

      private: // widgets
        Ui::LosAgentUi *ui;

      private: // data
        QMap<QString, QStringList> L_providerModels; // 厂商名 -> 模型列表 (来自后端 list_providers)
    };

} /* namespace LosView */
