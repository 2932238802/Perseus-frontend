// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QLineEdit>
#include <QWidget>


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