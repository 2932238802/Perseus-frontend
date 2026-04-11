#pragma once
#include "core/LosLog/LosLog.h"
#include "view/style/LosCommandArgsUi_style.h"
#include <QDialog>
#include <QLineEdit>
#include <QList>
#include <QStringList>
#include <qlineedit.h>

namespace Ui
{
    class LosCommandArgsUi;
}

namespace LosView
{
    class LosCommandArgsUi : public QDialog
    {
        Q_OBJECT
      public:
        explicit LosCommandArgsUi(QWidget *parent = nullptr);
        ~LosCommandArgsUi();

      private slots:
        void onExeScript(const QString &script_path, const QStringList &params);
        void onRunBtnClicked();
        void onBrowseBtnClicked();

      private: // init
        void initStyle();
        void initConnect();

      private:
        Ui::LosCommandArgsUi *ui;
        QList<QLineEdit *> L_paramsInputs;
        QString L_scriptPath;
    };

} // namespace LosView
