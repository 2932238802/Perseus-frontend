#pragma once

namespace LosCommon
{

/**
- 快捷键的设置
*/
namespace ShortCut
{
inline constexpr const char *FILE_SAVE           = "Ctrl+S";         // 保存当前文件
inline constexpr const char *FILE_SAVE_ALL       = "Ctrl+Shift+S";   // 保存所有文件
inline constexpr const char *FILE_OPEN           = "Ctrl+O";         // 打开文件/文件夹
inline constexpr const char *FILE_NEW            = "Ctrl+N";         // 新建文件
inline constexpr const char *TAB_CLOSE           = "Ctrl+W";         // 关闭当前标签页
inline constexpr const char *TAB_NEXT            = "Ctrl+Tab";       // 切换到下一个标签页
inline constexpr const char *RUN_SINGLE_FILE     = "Ctrl+F5";        // 运行当前单文件
inline constexpr const char *BUILD_PROJECT       = "Ctrl+B";         // 编译整个工程 (或 F7)
inline constexpr const char *DEBUG_START         = "F5";             // 启动调试
inline constexpr const char *CODE_FORMAT         = "Ctrl+K, Ctrl+O"; // 格式化当前文档 (clang-format)
inline constexpr const char *LINE_COMMENT        = "Ctrl+/";         // 单行注释/取消注释
inline constexpr const char *BLOCK_COMMENT       = "Shift+Alt+A";    // 块注释 (/* ... */)
inline constexpr const char *LINE_DUPLICATE      = "Ctrl+Shift+D";   // 向下复制当前行
inline constexpr const char *LINE_DELETE         = "Ctrl+Shift+K";   // 删除当前行
inline constexpr const char *LINE_MOVE_UP        = "Alt+Up";         // 将当前行向上移动
inline constexpr const char *LINE_MOVE_DOWN      = "Alt+Down";       // 将当前行向下移动
inline constexpr const char *SEARCH_FIND         = "Ctrl+F";         // 呼出页内查找
inline constexpr const char *SEARCH_REPLACE      = "Ctrl+H";         // 呼出页内替换
inline constexpr const char *GOTO_LINE           = "Ctrl+G";         // 跳转到指定行
inline constexpr const char *GOTO_DEFINITION     = "F12";            // 跳转到定义
inline constexpr const char *TOGGLE_EXPLORER     = "Ctrl+Shift+E";   // 显示/隐藏左侧文件树
inline constexpr const char *TOGGLE_BOTTOM_PANEL = "Ctrl+J";         // 显示/隐藏底部面板 (Issues/Output)
inline constexpr const char *FOCUS_TERMINAL      = "Ctrl+`";         // 聚焦到底部终端
inline constexpr const char *FONT_ZOOM_IN        = "Ctrl+>";   // 放大字体 / 全局缩放增大
inline constexpr const char *FONT_ZOOM_OUT       = "Ctrl+<";   // 缩小
} // namespace ShortCut



/**
格式化文本的 format 内容
TODO: 之后 可以 自定义
*/
namespace LLVM_formatStyle
{
inline constexpr const char *FORMAT_STYLE = R"({
    Language: Cpp,
    BasedOnStyle: LLVM,
    ColumnLimit: 120,
    MaxEmptyLinesToKeep: 3,
    AlignConsecutiveAssignments: true,
    AlignTrailingComments: true,
    AlignConsecutiveMacros: true,
    PointerAlignment: Right,
    IndentWidth: 4,
    TabWidth: 4,
    UseTab: Never,
    AllowShortBlocksOnASingleLine: true,
    AllowShortFunctionsOnASingleLine: Inline,
    SortIncludes: true,
    BreakBeforeBraces: Allman
})";
} // namespace LLVM_formatStyle



/**
- 单个文件的运行
*/
namespace LosRunner_Constants
{
#ifdef Q_OS_WIN
inline constexpr const char *GXX = "g++.exe";
#else
inline constexpr const char *GXX = "g++";
#endif
inline constexpr const char *CXX_17     = "-std=c++17";
inline constexpr const char *CMD_OBJECT = "-o";
inline constexpr const char *LINUX_EXE  = ".out";
inline constexpr const char *WIN_EXE    = ".exe";


} // namespace LosRunner_Constants



namespace LosLsp_Constants
{
inline constexpr const char *CONTENT_LENGTH = "Content-Length: ";
inline constexpr const char *LSP_RNRN       = "\r\n\r\n";

} // namespace LosLsp_Constants



namespace LosFormatManager_Constants
{
inline constexpr const char *ASSUME_FILENAME_ASRS = "-assume-filename=";
inline constexpr const char *STYLE_ASRS           = "-style=";
#ifdef Q_OS_WIN
inline constexpr const char *CLANG_FORMAT = "clang-format.exe";
#else
inline constexpr const char *CLANG_FORMAT = "clang-format";
#endif

} // namespace LosFormatManager_Constants



namespace LosConfig_Constants
{
inline constexpr const char *CMAKE                   = "cmake";
inline constexpr const char *SRC_DIR                 = ".";
inline constexpr const char *CMD_SOURCE              = "-S";
inline constexpr const char *CMD_TARGET              = "-B";
inline constexpr const char *BUILD_NAME              = "build";
inline constexpr const char *COMPILE_COMMANDS_OPTION = "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON";
inline constexpr const char *COMPILE_COMMANDS_FILE   = "compile_commands.json";
} // namespace LosConfig_Constants

} // namespace LosCommon
