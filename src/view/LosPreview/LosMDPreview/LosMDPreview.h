//
// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include "view/LosPreview/LosPreview.h"
#include <QObject>
#include <QUrl>
#include <QWidget>
#include <qtextbrowser.h>
#include <qwidget.h>
namespace LosView
{
    class LosMDPreview : public LosPreview
    {
        Q_OBJECT
      public:
        explicit LosMDPreview(QWidget *parent = nullptr);

      public: // tool
        void render(const QString &content) override;

      private slots: // chs
        void onAnchorClicked(const QUrl &url);

      private: // params
        QTextBrowser *L_browser = nullptr;
    };
} // namespace LosView