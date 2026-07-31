// Copyright (c) 2026 LosAngelous (shengjie.lin)


#pragma once
#include "common/constants/ConstantsClass/LosToolChainClass.h"

#include <QDialog>
#include <qtmetamacros.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LosSettingsUi;
}
QT_END_NAMESPACE

class QShowEvent;

namespace LosView
{
    class LosSettingsUi : public QDialog
    {
        Q_OBJECT
      public: // construct
        explicit LosSettingsUi(QWidget *parent = nullptr);
        ~LosSettingsUi();

      protected:
        void showEvent(QShowEvent *event) override;

      private: // init
        void initStyle();
        void initConnect();
        void updateCategoryListWidth(); // 更新宽度
        void centerOnParent();          // 居中 界面

      private: // private tool
        void setInstalled(LosCommon::LosToolChain_Constants::LosTool tool);
        void initThemePage();
        void initFontPage();
        void initFormatPage();

      private slots: // chs
        void onSaveSettings();
        void onFindExePath(const QString &file_path, LosCommon::LosToolChain_Constants::LosTool tool);
        void onThemeComboChanged(int index);
        void onFontComboChanged(int index);
        void onFormatReload();
        void onFormatSave();

      public slots: // chs
        void onCMakeInstallBtnClicked();

      private:
        bool L_hasInitialPosition = false;
        Ui::LosSettingsUi *ui;
    };
}; /* namespace LosView */