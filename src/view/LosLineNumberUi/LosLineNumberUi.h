// Copyright (c) 2026 LosAngelous (shengjie.lin)

#pragma once
#include <qevent.h>
#include <qwidget.h>

namespace LosView
{
    class LosEditorUi;
}

namespace LosView
{
    class LosLineNumberUi : public QWidget
    {
      public:
        explicit LosLineNumberUi(LosEditorUi *line);
        ~LosLineNumberUi() = default;

      public: /* tool */
        QSize sizeHint() const override;

      protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;

      private:
        LosEditorUi *LOS_editor;
    };

} /* namespace LosView */