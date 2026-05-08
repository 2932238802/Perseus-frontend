#pragma once
#include <qpushbutton.h>

namespace LosCommon
{
    inline QPushButton *GetPushBtn(const QString &name, const QString &style_sheet, const QSize &size)
    {
        QPushButton *btn = new QPushButton(name);
        btn->setStyleSheet(style_sheet);
        btn->setFixedSize(size);
        return btn;
    }
} // namespace LosCommon
