// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include <QJsonObject>

namespace LosCore
{
    class LosLspCMake : public LosLspClient
    {
        Q_OBJECT
      public:
        explicit LosLspCMake(QObject *parent = nullptr);
        void start(const QStringList &start_up_args, const QString &exe_path) override;
        void dealLspMessage(const QJsonObject &obj) override;
        ~LosLspCMake() override = default;

      private: // init
        void initConnect();

      private: // tool
        void sendInitializeRequest();
        void sendInitializeMsg();
    };


} /* namespace LosCore */