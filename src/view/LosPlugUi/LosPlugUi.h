#pragma once
#include "core/LosRouter/LosRouter.h"

#include <QWidget>
#include <qlistwidget.h>
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

      public: /* tool */
        bool isEmpty();

      private:
        void initConnect();

      private slots:
        void onPluginReply(const QList<LosCommon::LosNet_Constants::PluginInfo> &plugins);
        void onPluginItemClicked(int index);

      private:
        QList<LosCommon::LosNet_Constants::PluginInfo> LOS_infos;
        Ui::LosPlugUi *ui;
    };

} /* namespace LosView */
