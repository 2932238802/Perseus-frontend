
#pragma once

#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosRouter/LosRouter.h"

namespace LosCore
{
    class LosLspPython : public LosLspClient
    {
        Q_OBJECT
      public:
        explicit LosLspPython(QObject *parent);
        ~LosLspPython() override = default;
        void start(const QStringList &start_up_args, const QString &exe_path) override;
        void dealLspMessage(const QJsonObject &obj) override;


      private: // tool
        void sendInitializeRequest();
        void sendInitializeMsg();


      private:
    };
} // namespace LosCore
