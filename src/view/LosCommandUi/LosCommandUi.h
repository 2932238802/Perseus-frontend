#pragma once

#include "common/constants/ConstantsClass.h"
#include "core/LosLog/LosLog.h"
#include "core/LosRouter/LosRouter.h"
#include "view/style/LosCommandUi_style.h"
#include <QDir>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QString>
#include <qboxlayout.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <qwidget.h>

namespace LosView
{
    class LosCommandUi : public QDialog
    {
        Q_OBJECT
      public:
        explicit LosCommandUi(QWidget *parent = nullptr);
        ~LosCommandUi() = default;

      public:
        void showPalette();

      private: /* init */
        void initStyle();
        void initConnect();
        void initScanLocalPlugins();

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

      private:
        QLineEdit *L_searchBox;
        QListWidget *L_lists;
        QMap<QString, LosCommon::LosCommandUi_Constants::CommandsInfo> L_commands;
    };
} /* namespace LosView */
