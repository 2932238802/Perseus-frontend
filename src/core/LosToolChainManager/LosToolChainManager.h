#pragma once
#include "common/constants/ConstantsClass.h"
#include <QHash>
#include <QObject>
#include <qtmetamacros.h>

namespace LosCore
{
class LosToolChainManager : public QObject
{
    Q_OBJECT

  public: // tool
    explicit LosToolChainManager(QObject *parent = nullptr);
    ~LosToolChainManager() = default;

  public slots:
    // void onCheckToolchain(LosCommon::LosToolChain_Constants::LosLanguage);

  private:
    void initConfig();

  private:
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, LosCommon::LosToolChain_Constants::ToolChainConfig>
        LOS_languageToConfig;
    QHash<LosCommon::LosToolChain_Constants::LosLanguage, QString> L_activePath;
};
} // namespace LosCore