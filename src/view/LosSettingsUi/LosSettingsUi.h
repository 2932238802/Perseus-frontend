// Copyright (c) 2026 LosAngelous (shengjie.lin)


#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include <QComboBox>
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <memory>
#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LosSettingsUi;
}
QT_END_NAMESPACE

namespace LosView
{
    class LosSettingsUi : public QDialog
    {
        Q_OBJECT
      public: // construct
        explicit LosSettingsUi(QWidget *parent = nullptr);
        ~LosSettingsUi();

      private: // init
        void initStyle();
        void initConnect();

      private: // private tool
        void setInstalled(LosCommon::LosToolChain_Constants::LosTool tool);
        void initThemePage();
        void initFormatPage();

      private slots: // chs
        void onSaveSettings();
        void onFindExePath(const QString &file_path, LosCommon::LosToolChain_Constants::LosTool tool);
        void onThemeComboChanged(int index);
        void onFormatReload();
        void onFormatSave();

      public slots: // chs
        void onCMakeInstallBtnClicked();

      private:
        Ui::LosSettingsUi *ui;
    };
}; /* namespace LosView */