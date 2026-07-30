// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QLineEdit>
#include <QWidget>


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