// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QSet>
#include <QTabWidget>

namespace LosView
{
    class LosTerminalTabsUi : public QTabWidget
    {
        Q_OBJECT
      public:
        explicit LosTerminalTabsUi(QWidget *parent = nullptr);
        ~LosTerminalTabsUi() = default;

      private:
        void initStyle();
        void initConnect();

      private: /* tool */
        void addNewTerminal();

      private:
        QSet<int> L_usedTerminalIds;
    };
} /* namespace LosView */
