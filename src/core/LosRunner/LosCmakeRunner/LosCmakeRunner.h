// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include <QObject>
#include <QProcess>
#include <optional>
#include <qtmetamacros.h>

namespace LosCore
{

    class LosCmakeRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public: // construct
        explicit LosCmakeRunner(QObject *parent = nullptr);
        ~LosCmakeRunner() override;

      public: // tool
        void start(const QString &path = "build") override;
        void stop() override;

      public: // set
        void setCMakeExe(const QString &file_path);

      private: // init
        void initConnect();
        std::optional<QString> findLatestIndex(const QString &cur_file_path);

      private: // tool
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