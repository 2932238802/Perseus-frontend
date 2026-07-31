// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once

#include "core/LosTerminal/LosTerminal.h"

#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>
#include <qtmetamacros.h>

class QColor;
class QString;

namespace LosView
{
    class LosTerminalUi : public QWidget
    {
        Q_OBJECT
      public: // construct
        explicit LosTerminalUi(QWidget *parent = nullptr);
        ~LosTerminalUi();

      private: // init
        void initStyle();
        void initConnect();

      private: // theme
        QColor themeBackground() const;
        QString themeForeground() const;

      private: // param
        QWebEngineView *L_webView;
        QWebChannel *L_channel;
        LosCore::LosTerminal *L_backend;
    };

} /* namespace LosView */