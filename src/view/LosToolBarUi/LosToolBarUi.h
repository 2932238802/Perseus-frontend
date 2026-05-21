#pragma once

#include "view/LosDropdownButtonUi/LosDropdownButtonUi.h"
#include <qtmetamacros.h>
#include <qwidget.h>


namespace LosView
{
    class LosToolBarUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosToolBarUi(QWidget *parent = nullptr);
        ~LosToolBarUi();

      private: // init
        void initStyle();
        void initFilesBtn(LosView::LosDropdownButtonUi* btn);
    };
} // namespace LosView
