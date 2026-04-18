
#pragma once

namespace LosCommon
{
    /** 
     * 全局快捷键设置
     */
    namespace ShortCut
    {
        inline constexpr const char *FILE_SAVE           = "Ctrl+S";         /* 保存当前文件 */
        inline constexpr const char *FILE_SAVE_ALL       = "Ctrl+Shift+S";   /* 保存所有文件 */
        inline constexpr const char *COMMANDS            = "Ctrl+Shift+P";   /* 打开指令 */
        inline constexpr const char *FILE_OPEN           = "Ctrl+O";         /* 打开文件/文件夹 */
        inline constexpr const char *FILE_NEW            = "Ctrl+N";         /* 新建文件 */
        inline constexpr const char *TAB_CLOSE           = "Ctrl+W";         /* 关闭当前标签页 */
        inline constexpr const char *TAB_NEXT            = "Ctrl+Tab";       /* 切换到下一个标签页 */
        inline constexpr const char *RUN_SINGLE_FILE     = "Ctrl+F5";        /* 运行当前单文件 */
        inline constexpr const char *BUILD_PROJECT       = "Ctrl+B";         /* 编译整个工程 (或 F7) */
        inline constexpr const char *DEBUG_START         = "F5";             /* 启动调试 */
        inline constexpr const char *CODE_FORMAT         = "Ctrl+K, Ctrl+O"; // 格式化当前文档 (clang-format)
        inline constexpr const char *LINE_COMMENT        = "Ctrl+/";         /* 单行注释/取消注释 */
        inline constexpr const char *BLOCK_COMMENT       = "Shift+Alt+A";    // 块注释 (/* ... */)
        inline constexpr const char *LINE_DUPLICATE      = "Ctrl+Shift+D";   /* 向下复制当前行 */
        inline constexpr const char *LINE_DELETE         = "Ctrl+X";         /* 删除当前行 */
        inline constexpr const char *LINE_MOVE_UP        = "Alt+Up";         /* 将当前行向上移动 */
        inline constexpr const char *LINE_MOVE_DOWN      = "Alt+Down";       /* 将当前行向下移动 */
        inline constexpr const char *SEARCH_FIND         = "Ctrl+F";         /* 呼出页内查找 */
        inline constexpr const char *SEARCH_REPLACE      = "Ctrl+H";         /* 呼出页内替换 */
        inline constexpr const char *GOTO_LINE           = "Ctrl+G";         /* 跳转到指定行 */
        inline constexpr const char *GOTO_DEFINITION     = "F12";            /* 跳转到定义 */
        inline constexpr const char *TOGGLE_EXPLORER     = "Ctrl+Shift+E";   /* 显示/隐藏左侧文件树 */
        inline constexpr const char *TOGGLE_BOTTOM_PANEL = "Ctrl+J";         /* 显示/隐藏底部面板 */
        inline constexpr const char *FOCUS_TERMINAL      = "Ctrl+`";         /* 聚焦到底部终端 */
        inline constexpr const char *FONT_ZOOM_IN        = "Ctrl+>";         /* 放大字体 / 全局缩放增大 */
        inline constexpr const char *FONT_ZOOM_OUT       = "Ctrl+<";         /* 缩小字体 */
    } /* namespace ShortCut */
}