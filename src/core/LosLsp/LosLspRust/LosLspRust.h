#pragma once
#include "common/util/DebugPJson.h"
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosState/LosState.h"
#include "models/LosFileNode/LosFileNode.h"
#include <QCoreApplication>
#include <QDir>
#include <QObject>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qprocess.h>
#include <qtmetamacros.h>

namespace LosCore
{

class LosLspRust : public LosLspClient
{
    Q_OBJECT
  public: // construct
    explicit LosLspRust(QObject *parent = nullptr);
    ~LosLspRust() = default;

  public: // tool
    void start(const QStringList &start_up_args, const QString &exe_path) override;
    void dealLspMessage(const QJsonObject &obj) override;

  private:
    void sendInitializeRequest();
    void sendInitializedMsg();
    void initConnect();
};


} // namespace LosCore