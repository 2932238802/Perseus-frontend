// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include <QJsonObject>

namespace LosCore
{

    class LosLspClangd : public LosLspClient
    {
        Q_OBJECT
      public:
        explicit LosLspClangd(QObject *parent = nullptr);
        ~LosLspClangd() override = default;
        void start(const QStringList &start_up_args, const QString &exe_path) override;
        void dealLspMessage(const QJsonObject &obj) override;

      private:
        void initConnect();
        void sendInitializeRequest();
        void sendInitializedMsg();
    };

} /* namespace LosCore */
