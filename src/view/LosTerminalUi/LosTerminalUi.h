#pragma once

#include "core/LosTerminal/LosTerminal.h"
#include <QUrl>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>
#include <qwidget.h>
namespace LosView
{

class LosTerminalUi : public QWidget
{
  public:
    explicit LosTerminalUi(QWidget *parent = nullptr);
    ~LosTerminalUi();

  private: // init
    void initStyle();

  private:
    QWebEngineView *L_webView;
    QWebChannel *L_channel;
    LosCore::LosTerminal *L_backend;
};

} // namespace LosView