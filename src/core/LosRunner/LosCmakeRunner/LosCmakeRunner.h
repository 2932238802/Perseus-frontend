#pragma once
#include "common/constants/ConstantsStr/LosConfigStr.h"
#include "common/constants/ConstantsStr/LosRunnerStr.h"
#include "common/util/GetFilePath.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QDir>
#include <QObject>
#include <optional>
#include <qdebug.h>
#include <qfileinfo.h>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <qprocess.h>
#include <qtmetamacros.h>

namespace LosCore
{

    class LosCmakeRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public:
        explicit LosCmakeRunner(QObject *parent = nullptr);
        ~LosCmakeRunner() override;

      public: /* tool */
        void start(const QString &path = "build") override;
        void stop() override;

      public: /* set */
        void setCMakeExe(const QString &file_path);

      private: // init
        void initConnect();
        std::optional<QString> findLatestIndex(const QString &cur_file_path);

      private: /* tool */
        std::optional<QString> findExePath(const QString &build_path, const QString &targer_name);
        std::optional<QString> parseCMakeTarget(const QString &index, const QString &build_path);
        void onCMakeFinished(int exitCode, QProcess::ExitStatus exitStatus);


      private:
        QString L_curBuildPath;
        QString L_cmakeExe;
        QProcess *L_cmake;
        QProcess *L_runner;
    };

} /* namespace LosCore */