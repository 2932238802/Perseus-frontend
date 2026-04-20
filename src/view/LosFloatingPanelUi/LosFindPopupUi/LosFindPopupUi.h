#pragma once

#include "common/constants/ConstantsStr/LosFloatingPanelUiStr.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <qlineedit.h>
#include <qwidget.h>


namespace LosView
{

    class LosFindPopupUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosFindPopupUi(QWidget *parent = nullptr);
        ~LosFindPopupUi() = default;

      public: /* get */
        QString getInput() const;
        QLineEdit *getEdit() const;

      private:
        QLineEdit *L_lineEdit;
    };
} /* namespace LosView */