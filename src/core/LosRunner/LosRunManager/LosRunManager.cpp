// Copyright (c) 2026 LosAngelous (shengjie.lin)


#include "LosRunManager.h"
#include "common/constants/ConstantsStr/LosConfigStr.h"
#include "common/util/CheckLang.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosRunner/LosCmakeRunner/LosCmakeRunner.h"
#include "core/LosRunner/LosPythonRunner/LosPythonRunner.h"
#include "core/LosRunner/LosRustcRunner/LosRustcRunner.h"
#include "core/LosRunner/LosSingleCppRunner/LosSingleCppRunner.h"
#include "core/LosState/LosState.h"
#include "models/LosFilePath/LosFilePath.h"
#include <QDir>

namespace LosCore
{
    /**
     * @brief Construct a new Los Run Manager:: Los Run Manager object
     *
     * @param parent
     */
    LosRunManager::LosRunManager(QObject *parent) : QObject{parent}
    {
        initConnect();
    }
    LosRunManager::~LosRunManager()
    {
        stop();
    }



    /**
     * @brief execute
     *
     * @param file_path
     * @param is_project
     */
    void LosRunManager::execute(const QString &file_path, bool is_project)
    {
        auto lang = LosCommon::CheckLang(file_path);
        if (lang != LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
            L_mainEntryFilePath = file_path;

        auto &router = LosCore::LosRouter::instance();
        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            if (is_project)
            {
                if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::CMAKE))
                {
                    emit router._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::CMAKE);
                    return;
                }
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::CMAKE]->start(file_path);
            }
            else
            {
                if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS))
                {
                    emit router._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS);
                    return;
                }
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS]->start(file_path);
            }
            break;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
        {
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::RUSTC))
            {
                emit router._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::RUSTC);
                return;
            }
            LOS_runners[LosCommon::LosToolChain_Constants::LosTool::RUSTC]->start(file_path);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::PYTHON:
        {
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::PYTHON))
            {
                emit router._cmd_checkLanguageToolchain(lang, LosCommon::LosToolChain_Constants::LosTool::PYTHON);
                return;
            }
            LOS_runners[LosCommon::LosToolChain_Constants::LosTool::PYTHON]->start(file_path);
            break;
        }
        default:
        {
            break;
        }
        }
    }



    /**
     * @brief stop
     * 全部暂停
     */
    void LosRunManager::stop()
    {
        for (auto *runner : LOS_runners)
        {
            if (runner)
            {
                runner->stop();
            }
        }
    }



    /**
     * @brief onToolChainReady
     *
     * @param lan
     * @param tool
     * @param exePath
     */
    void LosRunManager::onToolChainReady(LosCommon::LosToolChain_Constants::LosLanguage lan, LosCommon::LosToolChain_Constants::LosTool tool,
                                         const QString &exePath)
    {
        INF("checking " + exePath + " ...", "LosRunManager::onToolChainReady");
        switch (tool)
        {
        case LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS:
        {
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS))
            {
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS] = new LosSingleCppRunner(this);
            }
            auto *runner = qobject_cast<LosSingleCppRunner *>(LOS_runners[LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS]);
            // 设置可执行 文件的位置
            runner->setExePath(exePath);
            runner->start(L_mainEntryFilePath);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::RUSTC:
        {
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::RUSTC))
            {
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::RUSTC] = new LosRustcRunner(this);
            }
            auto *runner = qobject_cast<LosRustcRunner *>(LOS_runners[LosCommon::LosToolChain_Constants::LosTool::RUSTC]);
            runner->setExePath(exePath);
            runner->start(L_mainEntryFilePath);
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::PYTHON:
        {
            INF("python!", "onToolChainReady");
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::PYTHON))
            {
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::PYTHON] = new LosPythonRunner(this);
            }
            auto *runner = qobject_cast<LosPythonRunner *>(LOS_runners[LosCommon::LosToolChain_Constants::LosTool::PYTHON]);
            runner->setExePath(exePath);
            runner->start(L_mainEntryFilePath);
            break;
        }
        default:
        {
            break;
        }
        }
    }



    /**
     * @brief onBuildToolReady
     * @param tool
     * @param exePath
     * @param args
     */
    void LosRunManager::onBuildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath, const QStringList &args)
    {
        if (LOS_runners.contains(tool))
        {
            return;
        }
        switch (tool)
        {
        case LosCommon::LosToolChain_Constants::LosTool::CMAKE:
        {
            LOS_runners[tool] = new LosCmakeRunner(this);
            auto runner       = qobject_cast<LosCmakeRunner *>(LOS_runners[tool]);
            runner->setCMakeExe(exePath);
            LosModel::LosFilePath porjectDir = LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);
            if (porjectDir.isExist())
            {
                runner->start(porjectDir.getFilePath() + QDir::separator() + LosCommon::LosConfig_Constants::BUILD_NAME);
            }
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::CARGO:
        {
            // cargo

            break;
        }
        default:
            break;
        }
    }



    /**
     * @brief initConnect
     * 连接
     */
    void LosRunManager::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_toolChainReady, this, &LosRunManager::onToolChainReady);
        connect(&router, &LosCore::LosRouter::_cmd_buildToolReady, this, &LosRunManager::onBuildToolReady);
    }


} /* namespace LosCore */