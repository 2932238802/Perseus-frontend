#pragma once
#include <concepts>
#include <qabstractbutton.h>
#include <qpushbutton.h>
#include <qstringliteral.h>
#include <qtoolbutton.h>
#include <qwidget.h>

namespace LosCommon
{
    template <class T>
    concept QtBtn = std::derived_from<T, QAbstractButton>;

    template <QtBtn btnType>
    inline btnType *GetBtn(const QString &name, const QString &style_sheet, const QSize &size, QWidget *parent)
    {
        btnType *btn = new btnType(parent);
        btn->setText(name);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(size);
        btn->setStyleSheet(style_sheet);
        return btn;
    }
} // namespace LosCommon
