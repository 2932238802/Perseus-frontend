// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "common/constants/ConstantsClass/LosEditorTabUiClass.h"

#include <QObject>


namespace LosView
{
    class LosEditorTabUi;
    class LosCommandUi;

    /**
     * @brief LosFindPopupManager
     * 承载三种弹窗之间的互相切换状态机（Ctrl+F / Ctrl+H / Ctrl+G）
     * 弹窗与编辑器之间的全部信号接线
     * 对外提供命令式入口：showFindPopup / findNext / findPrevious / showCommandPalette
     */
    class LosFindPopupManager : public QObject
    {
        Q_OBJECT
      public:
        explicit LosFindPopupManager(LosEditorTabUi *tab, QObject *parent = nullptr);

      public: /* tool */
        void showFindPopup(LosCommon::LosEditorTableUi_Constants::PopupKind kind);
        void findNext();
        void findPrevious();
        void showCommandPalette();

      private:
        LosEditorTabUi *L_tab;
        LosView::LosCommandUi *LOS_cmdPalette = nullptr; // 指令面板（Ctrl+Shift+P，懒创建）
        LosCommon::LosEditorTableUi_Constants::PopupKind L_pendingPopupKind =
            LosCommon::LosEditorTableUi_Constants::PopupKind::None; // 弹窗内按键切换的目标弹窗
    };
} /* namespace LosView */
