// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosCommandUiClass.h"

#include <QDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListWidget>
#include <QMap>
#include <QString>
#include <QWidget>
#include <qtmetamacros.h>

namespace LosView
{
    class LosCommandUi : public QDialog
    {
        Q_OBJECT
      public:
        explicit LosCommandUi(QWidget *parent = nullptr);
        ~LosCommandUi() = default;

      public:
        void showPalette(QWidget *anchor = nullptr);

      private: /* init */
        void initStyle();
        void initConnect();
        void initScanLocalPlugins();
        void applyZoomScale(); // 按 QApplication 字体等比缩放面板尺寸与内部字体

      private slots:
        void onSearchTextChanged(const QString &text);
        void executeSelectedItem();
        void onPluginPath(const QString &plugin_path);

      private:
        void regis(const QString &display, const LosCommon::LosCommandUi_Constants::CommandsInfo &cmd);

      protected slots:
        bool eventFilter(QObject *watched, QEvent *event) override;
        void focusOutEvent(QFocusEvent *event) override;
        bool event(QEvent *event) override;

      protected:
        void changeEvent(QEvent *event) override;

      private:
        QLineEdit *L_searchBox;
        QListWidget *L_lists;
        QMap<QString, LosCommon::LosCommandUi_Constants::CommandsInfo> L_commands;
    };
} /* namespace LosView */
