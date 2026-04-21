#include "view/LosPlugUi/LosPlugUi.h"
#include "ui_LosPlugUi.h"

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



    bool LosPlugUi::isEmpty()
    {
        return LOS_infos.isEmpty();
    }



    void LosPlugUi::initConnect()
    {
        auto &router = LosCore::LosRouter::instance();
        connect(&router, &LosCore::LosRouter::_cmd_net_pluginReply, this, &LosPlugUi::onPluginReply);
        connect(ui->extensionsList, &QListWidget::currentRowChanged, this, &LosPlugUi::onPluginItemClicked);
    }



    void LosPlugUi::onPluginReply(const QList<LosCommon::LosNet_Constants::PluginInfo> &plugins)
    {
        ui->extensionsList->clear();
        LOS_infos = plugins;
        for (const auto &info : plugins)
        {
            QString str = info.L_id + " | " + info.L_name + " | " + info.L_version;
            ui->extensionsList->addItem(str);
        }
    }



    void LosPlugUi::onPluginItemClicked(int index)
    {
        if (index < 0 || index >= LOS_infos.size())
        {
            return;
        }
        auto info = LOS_infos[index];
        emit LosCore::LosRouter::instance()._cmd_openPluginDetail(info);
    }

} /* namespace LosView */
