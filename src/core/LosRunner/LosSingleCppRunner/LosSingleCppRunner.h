// Copyright (c) 2026 LosAngelous (shengjie.lin)



#pragma once
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "models/LosFilePath/LosFilePath.h"

#include <QObject>
#include <QProcess>

namespace LosCore
{
    class LosSingleCppRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public: /* const */
        explicit LosSingleCppRunner(QObject *parent = nullptr);
        ~LosSingleCppRunner() override;

      public: /* tool */
        void start(const QString &path = "main.cpp") override;
        void stop() override;
        void setExePath(const QString &exe_path);

      private: /* init */
        void initConnect();

      private:
        LosModel::LosFilePath LOS_filePath;
        QString L_outPutPath;
        QString L_exePath;
        QProcess *L_gxxPro;
        QProcess *L_runPro;
    };
} /* namespace LosCore */