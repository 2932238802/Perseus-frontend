// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "common/constants/ConstantsClass/LosFloatingPanelUiClass.h"

#include <QDialog>

class QKeyEvent;

namespace LosView
{

    class LosFloatingPanelUi : public QDialog
    {
      public:
        explicit LosFloatingPanelUi(QWidget *contentWidget, bool auto_close, QWidget *parent);
        ~LosFloatingPanelUi() = default;

      private: /* init */
        void initStyle();

      public: /* tool */
        void showAtPosition(QWidget *parent, LosCommon::LosFloatingPanelUi_Constants::PositionMode mode);

      protected:
        /*
         * keyPressEvent
         */
        void keyPressEvent(QKeyEvent *event) override;

      private:
        QWidget *L_bgWidget;
    };

} /* namespace LosView */