// Copyright (c) 2026 LosAngelous (shengjie.lin)
#pragma once
namespace LosCommon
{
    namespace LosEditorTableUi_Constants
    {
        enum EditorType
        {
            CODE,    // 常规编辑器 代码编辑器
            PREVIEW, // 预览
            LOSEL,   // 以后的表格 后缀名为 sel
        };

        enum class PopupKind
        {
            None,     // 无待切换目标
            Find,     // 查找弹窗 (Ctrl+F)
            Replace,  // 替换弹窗 (Ctrl+H)
            GotoLine, // 跳转行弹窗 (Ctrl+G)
        };
    } // namespace LosEditorTableUi_Constants
} // namespace LosCommon
