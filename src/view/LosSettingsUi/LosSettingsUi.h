
#pragma once
#include "core/LosLog/LosLog.h"
#include <QDialog>
#include <QPushButton>
#include <memory>
#include <qdialog.h>
#include <qdialogbuttonbox.h>
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
      public:
        explicit LosSettingsUi(QWidget *parent = nullptr);
        ~LosSettingsUi();

      private:
        void initStyle();
        void initConnect();

      private slots:
        void onSaveSettings();

      private:
        Ui::LosSettingsUi *ui;
    };
}; /* namespace LosView */