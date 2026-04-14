#pragma once
#include "common/util/DebugPJson.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <qcoreapplication.h>
#include <qtmetamacros.h>

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


      private: /* init */
        void initConnect();

      private: /* tool */
        void sendInitializeRequest();
        void sendInitializeMsg();
    };


} /* namespace LosCore */