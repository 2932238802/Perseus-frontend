#pragma once
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include <qglobal.h>
#include <qprocess.h>
#include <qtmetamacros.h>
#include <utility>

namespace LosCore
{
    class LosScriptRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public:
        explicit LosScriptRunner(QObject *parent = nullptr);
        ~LosScriptRunner() ;
        void start(const QString &main_file_path = "") override;
        void stop() override;

      private: // init
        void initConnect();

      private:
        QString L_workingDir;
        QStringList L_args;
        QProcess *L_runner;
    };

} // namespace LosCore