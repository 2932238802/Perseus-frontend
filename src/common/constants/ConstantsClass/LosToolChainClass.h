#pragma once
#include <QString>
#include <QStringList>

namespace LosCommon
{
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


} // namespace LosCommon