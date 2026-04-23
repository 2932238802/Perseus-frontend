#include "LosLineNumberUi.h"

#include "view/LosEditorUi/LosEditorUi.h"

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
        if (event->button() == Qt::LeftButton)
        {
            int y              = event->pos().y();
            QTextCursor cursor = LOS_editor->cursorForPosition(QPoint(0, y));
            int line           = cursor.blockNumber();
            LOS_editor->gotoLine(line);
        }
        QWidget::mousePressEvent(event);
    }


} /* namespace LosView */