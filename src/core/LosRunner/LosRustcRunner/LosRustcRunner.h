// Copyright (c) 2026 LosAngelous (shengjie.lin)


#pragma once

#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QObject>
#include <QProcess>

namespace LosCore
{
    class LosRustcRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public: /* constrcut */
        explicit LosRustcRunner(QObject *parent = nullptr);
        ~LosRustcRunner();

      public: /* tool */
        void stop() override;
        void start(const QString &file_path) override;
        void setExePath(const QString &exe_path);

      public: /* init */
        void initConnect();

      private:                                    /* params */
        LosModel::LosFilePath LOS_targetFilePath; /* 针对的 单个 rust文件 */
        LosModel::LosFilePath L_outPutPath;       /* 最后 输出的文件 */
        LosModel::LosFilePath L_exePath;          /* 可执行文件的位置 */
        QProcess *L_rustc;                        /* rustc */
        QProcess *L_runner;                       /* 运行可执行文件 */
    };

} /* namespace LosCore */