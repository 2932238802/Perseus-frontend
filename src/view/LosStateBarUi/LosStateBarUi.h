// Copyright (c) 2026 LosAngelous (shengjie.lin)

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

      public: // tool
        void setCodingBarLabel(const QString &text);
        void setLangLabel(const QString &text);

      private:
        void initStyle();
        void initConnect();

      private:
        QLabel *L_gitPath      = nullptr;
        QLabel *L_codingLabel  = nullptr;
        QLabel *L_messageLabel = nullptr;
        QLabel *L_langLabel    = nullptr;
    };
} // namespace LosView
