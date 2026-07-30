// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <functional>
#include <qaction.h>
#include <qmenu.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>

namespace LosView
{
    class LosDropdownButtonUi : public QPushButton
    {
        Q_OBJECT
      public:
        explicit LosDropdownButtonUi(QWidget *parent = nullptr);
        explicit LosDropdownButtonUi(const QString &, QWidget *parent = nullptr);
        QAction *addOption(const QString &, std::function<void()>);
        void addSeparator();
        QMenu *getMenu() const;

      private: /* init */
        void initMenu();

      private: /* params */
        QMenu *L_menu;
    };
} /* namespace LosView */