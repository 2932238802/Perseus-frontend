#pragma once
#include "core/LosLsp/LosLspClient/LosLspClient.h"
#include "core/LosRouter/LosRouter.h"
#include <QCoreApplication>
#include <QDir>
#include <qjsonarray.h>
#include <qjsonobject.h>

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

  private:
};

} // namespace LosCore
