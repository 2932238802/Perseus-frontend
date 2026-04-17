#pragma once
#include "common/constants/ConstantsClass.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosAbstractRunner/LosAbstractRunner.h"
#include <qglobal.h>
#include <qprocess.h>
#include <qtmetamacros.h>
#include <utility>

namespace LosCore
{
    /**
     * @brief
     * LosScriptRunner
     * - 脚本运行类
     *   - 运行插件的脚本
     *   - 运行内置安装编译器的脚本
     * - Perseus持有唯一
     *   - initConnect() 进行对应的请求处理
     *   - 与Perseus解耦
     *
     * onAutoInstallTool
     * - 处理安装编译器的请求
     * - 运行对应的脚本
     *
     *
     */
    class LosScriptRunner : public LosAbstractRunner
    {
        Q_OBJECT
      public:
        explicit LosScriptRunner(QObject *parent = nullptr);
        ~LosScriptRunner();
        void start(const QString &main_file_path = "") override;
        void stop() override;

      private: // init
        void initConnect();

      private: // get
        QString getScriptsInstallDir();

      public slots:
        void onAutoInstallTool(const LosCommon::LosToolChain_Constants::ToolChainConfig &);

      private:
        QString L_workingDir;
        QStringList L_args;
        QProcess *L_runner;
    };

} /* namespace LosCore */