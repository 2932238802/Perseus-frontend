#pragma once


#include "core/LosLog/LosLog.h"
#include <QTabBar>
#include <QTimer>
#include <QToolButton>
#include <qobject.h>
#include <qtabwidget.h>
#include <qwidget.h>

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

      private: /* tool */
        void addNewTerminal();

      private:
        QSet<int> L_usedTerminalIds;
    };
} /* namespace LosView */
