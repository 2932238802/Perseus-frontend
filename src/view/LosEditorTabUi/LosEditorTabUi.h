
#pragma once
#include "common/constants/ConstantsClass.h"
#include "common/util/CheckLang.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "core/LosShortcutManager/LosShortcutManager.h"
#include "models/LosFileContext/LosFileContext.h"
#include "models/LosFilePath/LosFilePath.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include "view/LosPluginDetailUi/LosPluginDetailUi.h"

#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QWidget>
#include <qaction.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qglobal.h>
#include <qmenu.h>
#include <qmessagebox.h>
#include <qset.h>
#include <qsharedpointer.h>
#include <qstringliteral.h>
#include <qtabbar.h>
#include <qtablewidget.h>
#include <qtabwidget.h>
#include <qtextcursor.h>
#include <qwidget.h>

namespace LosView
{
    class LosEditorTabUi : public QWidget
    {
        Q_OBJECT
      public: /* construct */
        explicit LosEditorTabUi(QTabWidget *tabWidget, QWidget *object = nullptr);
        ~LosEditorTabUi() override;

      public: /* tool */
        void closeTab(int index);
        void closeAllTabs();
        void saveTab();
        void saveAllTabs();
        void openFile(const LosModel::LosFilePath &file);
        void formatTab();

      public: /* get */
        LosEditorUi *getCurEditor();
        int tabCount() const;
        QString getCurFilePath() const;
        QStringList getOpenFiles() const;
        int getCurEditIndex() const;

      private slots: /* chs */
        void onTabCloseRequested(int index);
        void onEditDirty(const QString &file_path, bool is_dirty);
        void onDefineResult(const QString &file_path, int line);
        void onDoubleClickedOnIssue(const QString &file_path, int line);
        void onResetCheck(LosCommon::LosToolChain_Constants::LosLanguage lan, const QString &curFile = "");
        void onTabClicked(int index);
        void onOpenPlugin(const LosCommon::LosNet_Constants::PluginInfo &info);
        void onFileRenamed(const QString &old_path, const QString &new_path);

      private: /* init */
        void initConnect();
        void initTabBar();


      private: /* tool */
        void checkLspAnsFormat(const QString &file_path);


      private: /* params */
        QSet<LosCommon::LosToolChain_Constants::LosLanguage> L_checkedLanguage;
        QTabWidget *L_tabWidget;
        QMap<QString, LosView::LosEditorUi *> LOS_pathToUi;
    };
} /* namespace LosView */
