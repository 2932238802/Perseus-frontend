
#include "LosRunManager.h"
#include "common/constants/ConstantsClass.h"
#include "core/LosRunner/LosRustcRunner/LosRustcRunner.h"


namespace LosCore
{
    /*
     * - 构造函数
     */
    LosRunManager::LosRunManager(QObject *parent) : QObject{parent}
    {
        initConnect();
    }
    LosRunManager::~LosRunManager()
    {
        stop();
    }



    /*
     * 执行函数
     */
    void LosRunManager::execute(const QString &file_path, bool is_project)
    {
        auto lang = LosCommon::CheckLang(file_path);
        if (lang != LosCommon::LosToolChain_Constants::LosLanguage::UNKNOWN)
            L_mainEntryFilePath = file_path;

        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            if (is_project)
            {
                if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::CMAKE))
                {
                    emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                        lang, LosCommon::LosToolChain_Constants::LosTool::CMAKE);
                    return;
                }
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::CMAKE]->start(file_path);
            }
            else
            {
                if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS))
                {
                    emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                        lang, LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS);
                    return;
                }
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS]->start(file_path);
            }
            break;
        }
        case LosCommon::LosToolChain_Constants::LosLanguage::RUST:
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::RUSTC))
            {
                emit LosCore::LosRouter::instance()._cmd_checkLanguageToolchain(
                    lang, LosCommon::LosToolChain_Constants::LosTool::RUSTC);
                return;
            }
            LOS_runners[LosCommon::LosToolChain_Constants::LosTool::RUSTC]->start(file_path);
            break;
        default:
        {
            break;
        }
        }
    }



    /*
     * - 全部暂停
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



    /*
     * - 工具链 已完备
     */
    void LosRunManager::onToolChainReady(LosCommon::LosToolChain_Constants::LosLanguage lan,
                                         LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath)
    {
        switch (tool)
        {
        case LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS:
        {
            if (!LOS_runners.contains(LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS))
            {
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS] = new LosSingleCppRunner(this);
            }
            auto *runner = qobject_cast<LosSingleCppRunner *>(
                LOS_runners[LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS]);
            /*
             * 设置可执行 文件的位置
             */
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
            auto *runner =
                qobject_cast<LosRustcRunner *>(LOS_runners[LosCommon::LosToolChain_Constants::LosTool::RUSTC]);
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



    /*
     * - 构建工具 完毕
     */
    void LosRunManager::onBuildToolReady(LosCommon::LosToolChain_Constants::LosTool tool, const QString &exePath,
                                         const QStringList &args)
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
            LosModel::LosFilePath porjectDir =
                LosState::instance().get<LosModel::LosFilePath>(LosCommon::LosState_Constants::SG_STR::PROJECT_DIR);
            if (porjectDir.isExist())
            {
                runner->start(porjectDir.getFilePath() + QDir::separator() +
                              LosCommon::LosConfig_Constants::BUILD_NAME);
            }
            break;
        }
        case LosCommon::LosToolChain_Constants::LosTool::CARGO:
        {
            /*
             * cargo
             */
            break;
        }
        default:
            break;
        }
    }



    /*
     * 获取 当前的 执行器
     */
    LosAbstractRunner *LosRunManager::getCurRunner(const QString &file_path, bool is_project)
    {
        auto lang = LosCommon::CheckLang(file_path);
        switch (lang)
        {
        case LosCommon::LosToolChain_Constants::LosLanguage::CXX:
        {
            if (is_project)
            {
                return LOS_runners.value(LosCommon::LosToolChain_Constants::LosTool::CMAKE, nullptr);
            }
            else
            {
                return LOS_runners.value(LosCommon::LosToolChain_Constants::LosTool::G_PLUS_PLUS, nullptr);
            }
        }
        default:
            break;
        }

        return nullptr;
    }



    /*
     * 连接
     */
    void LosRunManager::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_toolChainReady, this, &LosRunManager::onToolChainReady);
        connect(&router, &LosCore::LosRouter::_cmd_buildToolReady, this, &LosRunManager::onBuildToolReady);
    }


} /* namespace LosCore */