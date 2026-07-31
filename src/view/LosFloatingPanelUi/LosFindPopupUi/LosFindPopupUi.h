// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QWidget>


namespace LosView
{

    class LosFindPopupUi : public QWidget
    {
      public:
        explicit LosFindPopupUi(QWidget *parent = nullptr);
        ~LosFindPopupUi() = default;

      public: /* get */
        QString getInput() const;
        QLineEdit *getEdit() const;

      public: // set
        void setMatchInfo(int current, int total);

      protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

      private:
        QLineEdit *L_lineEdit;
        QLabel *L_matchLabel;
    };
} /* namespace LosView */
