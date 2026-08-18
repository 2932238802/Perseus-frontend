// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosHoverPopup.h"

#include "common/constants/ConstantsNum/LosEditorUiNum.h"
#include "common/constants/ConstantsStr/LosEditorUiStr.h"

#include <QGuiApplication>
#include <QScreen>

namespace LosView
{
    LosHoverPopup &LosHoverPopup::instance()
    {
        static LosHoverPopup s;
        return s;
    }

    LosHoverPopup::LosHoverPopup()
        : QLabel(nullptr, Qt::ToolTip | Qt::FramelessWindowHint)
    {
        setTextFormat(Qt::RichText);
        setTextInteractionFlags(Qt::NoTextInteraction);
        setWordWrap(true);
        setMargin(LosCommon::LosEditorUi_Constants::HOVER_POPUP_MARGIN);
        setMaximumWidth(LosCommon::LosEditorUi_Constants::HOVER_POPUP_MAX_WIDTH);
        setAttribute(Qt::WA_ShowWithoutActivating, true);
        setFocusPolicy(Qt::NoFocus);
        setStyleSheet(LosCommon::LosEditorUi_Constants::HOVER_POP_STYLE);
    }

    void LosHoverPopup::showPopup(const QString &html, const QPoint &anchor, const QRect &wordRectGlobal)
    {
        setText(html);
        adjustSize();

        QPoint pos = anchor + QPoint(0, LosCommon::LosEditorUi_Constants::HOVER_SCREEN_MARGIN);
        QScreen *screen = QGuiApplication::screenAt(pos);
        if (!screen)
        {
            screen = QGuiApplication::primaryScreen();
        }
        QRect available = screen
                              ? screen->availableGeometry()
                              : QRect(0, 0, LosCommon::LosEditorUi_Constants::FALLBACK_SCREEN_W,
                                      LosCommon::LosEditorUi_Constants::FALLBACK_SCREEN_H);
        QSize popupSize = sizeHint();
        if (pos.x() + popupSize.width() > available.right())
        {
            pos.setX(available.right() - popupSize.width() - LosCommon::LosEditorUi_Constants::HOVER_SCREEN_MARGIN);
        }
        if (pos.x() < available.left() + LosCommon::LosEditorUi_Constants::HOVER_SCREEN_MARGIN)
        {
            pos.setX(available.left() + LosCommon::LosEditorUi_Constants::HOVER_SCREEN_MARGIN);
        }
        if (pos.y() + popupSize.height() > available.bottom())
        {
            int above = wordRectGlobal.top() - popupSize.height() - LosCommon::LosEditorUi_Constants::HOVER_SCREEN_MARGIN;
            pos.setY(qMax(above, available.top() + LosCommon::LosEditorUi_Constants::HOVER_SCREEN_MARGIN));
        }
        move(pos);
        show();
        raise();
    }

    void LosHoverPopup::hidePopup()
    {
        hide();
    }
} /* namespace LosView */
