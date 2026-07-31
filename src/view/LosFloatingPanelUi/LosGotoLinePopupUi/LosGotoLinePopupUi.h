// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QLineEdit>
#include <QWidget>

class QKeyEvent;


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

      protected:
        void keyPressEvent(QKeyEvent *event) override;

      private:
        QLineEdit *L_lineEdit;
    };
} /* namespace LosView */