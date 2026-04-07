#pragma once
#include "common/constants/ConstantsClass.h"
#include "core/LosRouter/LosRouter.h"
#include "ui_LosPlugUi.h"
#include <QWidget>
#include <qtmetamacros.h>

namespace Ui
{
    class LosPlugUi;
}

namespace LosView
{

    class LosPlugUi : public QWidget
    {
        Q_OBJECT

      public:
        explicit LosPlugUi(QWidget *parent = nullptr);
        ~LosPlugUi();

      private:
        void initConnect();

      private slots:
        void onPluginReply(const QList<LosCommon::LosNet_Constants::PluginInfo> &plugins);

      private:
        Ui::LosPlugUi *ui;
    };

} // namespace LosView
