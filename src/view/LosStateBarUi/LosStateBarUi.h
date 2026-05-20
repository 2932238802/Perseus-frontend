#pragma once
#include <QLabel>
#include <QWidget>
#include <qtmetamacros.h>

namespace LosView
{
    class LosStateBarUi : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosStateBarUi(QWidget *parent = nullptr);
        ~LosStateBarUi() = default;

      private:
        void initStyle();

      private:
        QLabel *L_messageLabel = nullptr;
        QLabel *L_langLabel    = nullptr;
    };
} // namespace LosView
