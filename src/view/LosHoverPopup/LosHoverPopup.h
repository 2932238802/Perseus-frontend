// Copyright (c) 2026 LosAngelous (shengjie.lin)
#pragma once

#include <QLabel>
#include <QPoint>
#include <QRect>
#include <QString>

namespace LosView
{
    /**
     * @brief 
     * 
     */
    class LosHoverPopup : public QLabel
    {
        Q_OBJECT
      private:
        LosHoverPopup();
        ~LosHoverPopup() override = default;
        Q_DISABLE_COPY(LosHoverPopup)

      public:
        static LosHoverPopup &instance();
        void showPopup(const QString &html, const QPoint &anchor, const QRect &wordRectGlobal);
        void hidePopup();
    };
} /* namespace LosView */
