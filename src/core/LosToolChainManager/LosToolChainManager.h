#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "common/util/DebugPJson.h"
#include "common/util/FindExePath.h"
#include "common/util/GetFilePath.h"
#include "common/util/GetSettingKey.h"
#include "core/LosRouter/LosRouter.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QJsonArray>
#include <QJsonParseError>
#include <QList>
#include <QObject>
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qjsonobject.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <qtmetamacros.h>

namespace LosCore
{
    class LosToolChainManager : public QObject
    {
        Q_OBJECT
      public: // construct
        explicit LosToolChainManager(QObject *parent = nullptr);
        ~LosToolChainManager() = default;

      public slots: // hsc
        void onCheckLanguageToolchain(LosCommon::LosToolChain_Constants::LosLanguage lang, LosCommon::LosToolChain_Constants::LosTool tool);
        void onCheckSingleTool(LosCommon::LosToolChain_Constants::LosTool tool);

      private: // init
        void initConfig();
        void initConnect();

      private: // private tool
        LosCommon::LosToolChain_Constants::ToolChainConfig
        parseToolNode(const QJsonObject &, const QString &,
                      LosCommon::LosToolChain_Constants::LosLanguage language = LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN);
        bool validateExecutable(const LosCommon::LosToolChain_Constants::ToolChainConfig &);


      private:
        QHash<LosCommon::LosToolChain_Constants::LosLanguage, QList<LosCommon::LosToolChain_Constants::LosTool>> LOS_languageToolMap;
        QHash<LosCommon::LosToolChain_Constants::LosTool, LosCommon::LosToolChain_Constants::ToolChainConfig> LOS_toolConfigs;
        QHash<LosCommon::LosToolChain_Constants::LosTool, QString> L_activeToolPath;
    };
} /* namespace LosCore */
