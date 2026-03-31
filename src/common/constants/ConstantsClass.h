#pragma once
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

/**
寻找文件的标识
*/
enum class FindFileType
{
    SYSTEM_TOOLCHAIN_CONFIG_JSON = 1,
    COMMON
};
}; // namespace LosCommon


namespace LosCommon
{

/**
LosLsp 就是 识别 一些语法 和 补全
diagnotisc DiagnosticSeverity 程度
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
    // 初始化
    REQ_INITIALIZE,
    // 语法补全
    REQ_COMPLETION,
    // 语法报错
    REQ_CHECK,
    REQ_HOVER,
    REQ_DEFINE,
};



struct PendingRequest
{
    QString L_filePath;
    QString L_content;
    QString L_langId;
};



/**
didChangeWatchedFiles 字段
changes 字段的
type 整形
*/
enum LspJson_didChangeWatchedFiles_changes_type
{
    Created = 1,
    Changed = 2
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
} // namespace LosLsp_Constants



/**
- 识别 CMAKE 等的常量
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
} // namespace LosConfig_Constants



/**
- 保留 会话 的固定字段
*/
namespace LosSession_Constants
{
struct Config
{
    QStringList L_curFilePaths{};
    QString L_curProDir = "";
};

} // namespace LosSession_Constants



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



enum class LosTool
{
    CMAKE,
    NINJA,
    GIT,
    G_PLUS_PLUS,
    CLANGD,
    CLANG_FORMAT,
    NEOCMAKELSP,
    UNKNOWN
};



enum class ToolCategory
{
    LSP,       // 语言服务器
    Compiler,  // 编译器
    Formatter, // 格式化工具
    Linter,    // 静态分析
    Debugger,  // 调试器
    BuildTool
};



struct ToolChainConfig
{
    LosLanguage LOS_language;     // 属于什么语
    ToolCategory L_category;      // 是什么类型的工具
    QString L_name;               // 工具标识名 (如 "clangd")
    QStringList L_exeName;        // 跨平台可执行文件名
    QStringList L_startupArgs;    // 默认启动参数
    bool L_requireNodeJs = false; // 是否依赖 Node 环境  有些需要
    QString L_version;            // 版本号
    QString L_downUrl;            // 下载直链
    QString L_validateKey;        // 文件的验证码 这个
};

} // namespace LosToolChain_Constants
} // namespace LosCommon
