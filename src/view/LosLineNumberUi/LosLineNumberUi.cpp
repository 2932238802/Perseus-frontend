#include "LosLineNumberUi.h"
#include <qevent.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qtextcursor.h>
#include <qwidget.h>

namespace LosView
{
    LosLineNumberUi::LosLineNumberUi(LosEditorUi *line) : QWidget(line), LOS_editor(line) {}



    /*
     * - 合适的  尺寸
     */
    QSize LosLineNumberUi::sizeHint() const
    {
        return QSize(LOS_editor->getLineNumberWidth(), 0);
    }



    /*
     * - 重绘
     */
    void LosLineNumberUi::paintEvent(QPaintEvent *event)
    {
        LOS_editor->lineNumberAreaPaintEvent(event);
    }



    /*
     * - 点击行号 进行定位
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