// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <qwidget.h>

namespace LosView
{
    class LosPreview : public QWidget
    {
        Q_OBJECT
      public:
        explicit LosPreview(QWidget *parent = nullptr);
        ~LosPreview() override = default;
        virtual void render(const QString &content) = 0;
    };
} // namespace LosView