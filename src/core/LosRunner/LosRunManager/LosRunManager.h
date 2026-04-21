#pragma once
#include "common/constants/ConstantsStr/LosConfigStr.h"
#include "common/util/CheckLang.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "core/LosRunner/LosCmakeRunner/LosCmakeRunner.h"
#include "core/LosRunner/LosPythonRunner/LosPythonRunner.h"
#include "core/LosRunner/LosRustcRunner/LosRustcRunner.h"
#include "core/LosRunner/LosSingleCppRunner/LosSingleCppRunner.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QObject>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

namespace LosCore
{

    class LosRunManager : public QObject
    {
        Q_OBJECT

      public:
        explicit LosRunManager(QObject *parent = nullptr);
        ~LosRunManager() override;

      public: /* tool */
        void execute(const QString &file_path, bool is_project = false);
        void stop();

      private slots:
        void onToolChainReady(LosCommon::LosToolChain_Constants::LosLanguage,
                              LosCommon::LosToolChain_Constants::LosTool tool, const QString &);
        void onBuildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                              const QStringList &args);

      private: /* init */
        void initConnect();

      private: /* param */
        QString L_mainEntryFilePath = "";
        QMap<LosCommon::LosToolChain_Constants::LosTool, LosAbstractRunner *> LOS_runners;
    };
} /* namespace LosCore */