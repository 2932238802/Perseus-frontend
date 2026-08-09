// Copyright (c) 2026 LosAngelous (shengjie.lin)

#include "LosLineNumberUi.h"

#include "common/constants/ConstantsNum/LosEditorUiNum.h"
#include "common/constants/ConstantsStr/LosEditorUiStr.h"
#include "view/LosEditorUi/LosEditorUi.h"
#include <qnamespace.h>
#include <qpoint.h>
#include <qtextcursor.h>

namespace LosView
{
    LosLineNumberUi::LosLineNumberUi(LosEditorUi *line) : QWidget(line), LOS_editor(line) {}



    /**
     * @brief sizeHint
     *
     * @return QSize
     */
    QSize LosLineNumberUi::sizeHint() const
    {
        return QSize(LOS_editor->getLineNumberWidth(), 0);
    }



    /**
     * @brief paintEvent
     *
     * @param event
     */
    void LosLineNumberUi::paintEvent(QPaintEvent *event)
    {
        LOS_editor->lineNumberAreaPaintEvent(event);
    }



    /**
     * @brief mousePressEvent
     *
     * @param event
     */
    void LosLineNumberUi::mousePressEvent(QMouseEvent *event)
    {
        if (!LOS_editor || event->button() != Qt::LeftButton)
        {
            QWidget::mousePressEvent(event);
            return;
        }
        const QPoint localPosition    = event->position().toPoint();
        const QPoint viewportPosition = LOS_editor->viewport()->mapFrom(this, localPosition);
        const int line                = LOS_editor->getBlockNumberByY(viewportPosition.y());
        if (line < 0)
        {
            event->accept();
            return;
        }
        const int markerLeft = width() - LosCommon::LosEditorUi_Constants::FOLD_MARKER_WIDTH;
        if (localPosition.x() >= markerLeft)
        {
            LOS_editor->toggleFold(line);
            event->accept();
            return;
        }
        LOS_editor->gotoLine(line);
        event->accept();
    }



} /* namespace LosView */