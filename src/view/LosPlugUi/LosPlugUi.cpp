#include "view/LosPlugUi/LosPlugUi.h"

namespace LosView
{

    LosPlugUi::LosPlugUi(QWidget *parent) : QWidget(parent), ui(new Ui::LosPlugUi)
    {
        ui->setupUi(this);
        initConnect();
    }

    LosPlugUi::~LosPlugUi()
    {
        delete ui;
    }


    void LosPlugUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_net_pluginReply, this, &LosPlugUi::onPluginReply);
    }



    void LosPlugUi::onPluginReply(const QList<LosCommon::LosNet_Constants::PluginInfo> &plugins)
    {
        ui->extensionsList->clear();
        for (const auto &info : plugins)
        {
            QString str = info.L_id + " | " + info.L_name + " | " + info.L_version;
            ui->extensionsList->addItem(str);
        }
    }

} // namespace LosView
