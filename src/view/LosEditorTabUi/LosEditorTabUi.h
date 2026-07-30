// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "common/constants/ConstantsClass/LosEditorTabUiClass.h"
#include "common/constants/ConstantsClass/LosNetClass.h"
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include "view/LosPreview/LosPreview.h"

#include <QMap>
#include <QObject>
#include <QSet>
#include <QTabWidget>
#include <QWidget>

namespace LosView
{
    class LosEditorTabUi : public QWidget
    {
        Q_OBJECT
      public: // construct
        explicit LosEditorTabUi(QTabWidget *tabWidget, QWidget *object = nullptr);
        ~LosEditorTabUi() override;

      public: // tool
        void closeTab(int index);
        void closeAllTabs();
        void saveTab();
        void saveAllTabs();
        void openFile(const LosModel::LosFilePath &file);
        void formatTab();

      public: // get
        QWidget *getCurEditor(LosCommon::LosEditorTableUi_Constants::EditorType = LosCommon::LosEditorTableUi_Constants::EditorType::CODE);
        int tabCount() const;
        QString getCurFilePath() const;
        QStringList getOpenFiles() const;
        int getCurEditIndex() const;

      private slots: // chs
        void onTabCloseRequested(int index);
        void onEditDirty(const QString &file_path, bool is_dirty);
        void onDefineResult(const QString &file_path, int line);
        void onDoubleClickedOnIssue(const QString &file_path, int line);
        void onResetCheck(LosCommon::LosToolChain_Constants::LosLanguage lan, const QString &curFile = "");
        void onTabClicked(int index);
        void onOpenPlugin(const LosCommon::LosNet_Constants::PluginInfo &info);
        void onFileRenamed(const QString &old_path, const QString &new_path);
        void onGotoLineShortcut();
        void onFindShortcut();
        void onTogglePreview(const QString &absolute_file_path); // 切换当前标签页的预览状态

      private: // init
        void initConnect();
        void initTabBar();
        void initShortCut();

      private: // tool
        void checkLspAnsFormat(const QString &file_path);
        void installCloseButton(int index);

      private: // params
        QSet<LosCommon::LosToolChain_Constants::LosLanguage> L_checkedLanguage;
        QTabWidget *L_tabWidget;
        QMap<QString, LosView::LosEditorUi *> LOS_pathToUi;     // 左侧是 absoluteFilePath 就是全路径
        QMap<QString, LosView::LosPreview *> LOS_pathToPreview; // 预览的逻辑
    };
} /* namespace LosView */
