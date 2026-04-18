#pragma once

#include "common/constants/ConstantsStr/LosFloatingPanelUiStr.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <qwidget.h>


namespace LosView
{

    class LosGotoLinePopupUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosGotoLinePopupUi(QWidget *parent = nullptr);
        ~LosGotoLinePopupUi() = default;

      public: /* get */
        int getLineNumber() const;
        QLineEdit *getLineEdit() const;

      private:
        QLineEdit *L_lineEdit;
    };
} /* namespace LosView */