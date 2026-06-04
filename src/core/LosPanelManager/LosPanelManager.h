// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosPanelManagerClass.h"
#include <QAbstractButton>
#include <QObject>
#include <QTabWidget>

namespace LosCore
{

    class LosPanelManager : public QObject
    {
        Q_OBJECT
      public:
        explicit LosPanelManager(QTabWidget *tab, QObject *parent = nullptr);
        ~LosPanelManager() = default;


      public: // tool
        void registerPanel(const QString &id, QWidget *widget, const QString &displayName,
                           int order); // 存在ui里面的 然后 登记在 管理器里面
        void addCloseBtn();
        void hidePanel(const QString& panel_id);

      private: // init
        void initCloseBtnConnect(QAbstractButton *btn, QWidget *parent);

      private:
        QHash<QString, LosCommon::LosPanelManager_Constants::Panel> LOS_panels; // key 是 panels的id
        QTabWidget *L_tab;
    };


} // namespace LosCore