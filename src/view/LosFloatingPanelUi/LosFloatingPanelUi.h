#pragma once
#include "common/constants/ConstantsClass/LosFloatingPanelUiClass.h"

#include <QDialog>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qevent.h>
#include <qnamespace.h>
#include <qwidget.h>

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