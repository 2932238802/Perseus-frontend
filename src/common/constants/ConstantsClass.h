#pragma once
#include "common/constants/ConstantsNum.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <functional>
#include <qglobal.h>

namespace LosCore
{
    class LosConfig;
}


namespace LosCommon
{

    /*
     * 寻找文件的标识
     */
    enum class FindFileType
    {
        SYSTEM_TOOLCHAIN_CONFIG_JSON = 1,
        COMMON_FILE,
        COMMON_DIR,
    };
}; /* namespace LosCommon */


namespace LosCommon
{
    /*
     * LosLsp 就是 识别 一些语法 和 补全
     * diagnotisc DiagnosticSeverity 程度
     */
    namespace LosLsp_Constants
    {
        enum DiagnosticSeverity
        {
            Error = 1,
            Warning,
            Information,
            Hint
        };



        enum LosLspType
        {
            /*
             * 初始化
             */
            REQ_INITIALIZE,
            /*
             * 语法补全
             */
            REQ_COMPLETION,
            /*
             * 语法报错
             */
            REQ_CHECK,
            REQ_HOVER,
            REQ_DEFINE,
            REQ_SEMANTIC_HIGHLIGHT
        };



        struct PendingRequest
        {
            QString L_filePath;
            QString L_content;
            QString L_langId;
        };



        /*
         * didChangeWatchedFiles 字段
         * changes 字段的
         * type 整形
         */
        enum LspJson_didChangeWatchedFiles_changes_type
        {
            Created = 1,
            Changed = 2,
            DELETE  = 3,
        };



        struct LosDiagnostic
        {
            int startLine;
            int startChar;
            int endLine;
            int endChar;
            DiagnosticSeverity ds = DiagnosticSeverity::Warning;
            QString message;
        };
    } /* namespace LosLsp_Constants */



    /*
     * - 识别 CMAKE 等的常量
     */
    namespace LosConfig_Constants
    {
        struct ConfigItem
        {
            QString L_name;
            QStringList L_filesToHotUpdate;
            std::function<bool(const QString &)> L_sniffer;
            std::function<LosCore::LosConfig *(QObject *)> L_creator;
        };
    } /* namespace LosConfig_Constants */



    /*
     * - 保留 会话 的固定字段
     */
    namespace LosSession_Constants
    {
        struct Config
        {
            QStringList L_curFilePaths{};
            QString L_curProDir     = "";
            QString L_curActiveFile = "";
        };

    } /* namespace LosSession_Constants */


    namespace LosToolChain_Constants
    {
        enum class LosLanguage
        {
            CXX,
            LUA,
            RUST,
            JAVA,
            PYTHON,
            CSHARP,
            CMAKE,
            UNKNOWN,
        };



        /*
         * PYTHON
         * - windows python
         * - Linux python3
         * - PYRIGHT 就是 语义 和 语法的高亮
         */
        enum class LosTool
        {
            CMAKE,
            NINJA,
            GIT,
            G_PLUS_PLUS,
            CLANGD,
            CLANG_FORMAT,
            NEOCMAKELSP,
            RUST_ANALYZER,
            CARGO,
            RUSTC,
            UNKNOWN,
            PYTHON,
            PYRIGHT,
        };



        enum class ToolCategory
        {
            LSP,       /* 语言服务器 */
            Compiler,  /* 编译器 */
            Formatter, /* 格式化工具 */
            Linter,    /* 静态分析 */
            Debugger,  /* 调试器 */
            BuildTool
        };



        /**
         * @brief config 下的tool信息
         * LOS_language 属于什么语
         * L_category 是什么类型的工具
         * L_name 工具标识名
         * L_exeName 跨平台可执行文件名
         * L_startupArgs  默认启动参数
         * L_requireNodeJs 是否依赖 Node 环境
         * L_version 版本号
         * L_downUrl 下载直链
         * L_validateKey 文件的验证码
         * L_scriptWin window安装脚本
         * L_scriptLinux Linux 安装脚本
         */
        struct ToolChainConfig
        {
            LosLanguage LOS_language;
            ToolCategory L_category;
            QString L_name;
            QStringList L_exeName;
            QStringList L_startupArgs;
            bool L_requireNodeJs = false;
            QString L_version;
            QString L_downUrl;
            QString L_validateKey;
            QString L_scriptWin;
            QString L_scriptLinux;
        };

    } /* namespace LosToolChain_Constants */



    namespace LosHighligher_Constants
    {
        struct SemanticToken
        {
            int L_startChar;
            int L_lenth;
            int L_tokenType;
        };
    } /* namespace LosHighligher_Constants */



    namespace LosFloatingPanelUi_Constants
    {
        enum class PositionMode
        {
            Center,
            TopRight,
            TopCenter,
        };
    }



    namespace LosNet_Constants
    {
        struct PluginInfo
        {
            QString L_id;
            QString L_name;
            QString L_version;
            QString L_downloadUrl;
            QString L_readmeUrl;
        };
    } /* namespace LosNet_Constants */


    namespace LosCommandUi_Constants
    {
        struct CommandsInfo
        {
            LosCommon::LosCommandUi_Constants::CommandsKind kind =
                LosCommon::LosCommandUi_Constants::CommandsKind::UNKOWN;
            QString L_scriptPath;
            QStringList L_params;
        };
    } /* namespace LosCommandUi_Constants */



    namespace LosPlatform_Constants
    {
        enum class OsType
        {
            WINDOWS = 1,
            LINUX_N = 2,
            LINUX_W = 3,
            MACOS,
            UNKOWN
        };
    }

} /* namespace LosCommon */
