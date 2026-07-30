// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "view/LosDropdownButtonUi/LosDropdownButtonUi.h"

#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>
#include <qtmetamacros.h>


namespace LosView
{
    /*
     * LosToolBarUi
     * - 顶部工具栏容器
     * - 控件全部由本类创建 方案 B
     * - 行为通过 LosRouter 信号 emit, 由 Perseus 监听处理
     *
     * 控件:
     *   icon_btn           "P" 图标
     *   files_btn          File 下拉菜单 (choose file / dir / version)
     *   view_btn           View 下拉菜单 (toggle Output / Issues / Terminal)
     *   setting_btn        Set
     *   run_btn            Run
     *   project_btn        CMake Pro? (radio)
     *   stretch            中间撑开
     *   project_dir_label 当前项目路径
     */
    class LosToolBarUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosToolBarUi(QWidget *parent = nullptr);
        ~LosToolBarUi() override;

      public: // public tool
        QLabel *getProjectDirLabel() const { return LOS_projectDirLabel; }

      private: /* init */
        void initStyle();
        void initFilesBtn();
        void initViewBtn();

      private: // private tool
        void registerToggle(const QString &title, int idx);

      private: /* params */
        QPushButton *LOS_iconBtn                   = nullptr;
        LosView::LosDropdownButtonUi *LOS_filesBtn = nullptr;
        LosView::LosDropdownButtonUi *LOS_viewBtn  = nullptr;
        QPushButton *LOS_settingBtn                = nullptr;
        QPushButton *LOS_runBtn                    = nullptr;
        QRadioButton *LOS_projectBtn               = nullptr;
        QLabel *LOS_projectDirLabel                = nullptr;
    };
} // namespace LosView
