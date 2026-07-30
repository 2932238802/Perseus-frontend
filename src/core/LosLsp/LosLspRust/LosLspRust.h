// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "core/LosLsp/LosLspClient/LosLspClient.h"

namespace LosCore
{
    class LosLspRust : public LosLspClient
    {
        Q_OBJECT
      public: /* construct */
        explicit LosLspRust(QObject *parent = nullptr);
        ~LosLspRust() = default;

      public: /* tool */
        void start(const QStringList &start_up_args, const QString &exe_path) override;
        void dealLspMessage(const QJsonObject &obj) override;

      private:
        void sendInitializeRequest();
        void sendInitializedMsg();
        void initConnect();
    };


} /* namespace LosCore */