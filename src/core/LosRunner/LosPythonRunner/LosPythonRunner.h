
#pragma once
#include "core/LosLog/LosLog.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QObject>
#include <QProcess>
#include <qprocess.h>
namespace LosCore
{

    class LosPythonRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public: // constract
        explicit LosPythonRunner(QObject *parent = nullptr);
        ~LosPythonRunner();

      public: // tool
        void start(const QString &main_file_path = "") override;
        void stop() override;

      public: // set
        void setExePath(const QString &file_path);

      private: // init
        void initConnect();

      private:
        QProcess *L_runner;
        QString L_exePath;
        LosModel::LosFilePath LOS_targetFile;
    };

} // namespace LosCore