#pragma once
#include <QFile>
#include <QString>
#include <QTextStream>

#include "core/LosLog/LosLog.h"

namespace LosStyle
{

    inline const QString perseus_getStyle()
    {
        QFile file(":/style/perseus_style.qss");

        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream stream(&file);
            return stream.readAll();
        }
        else
        {
            ERR("1", "1");
        }
        /*
         * 如果读取失败（通常是路径没写对），返回空字符串
         */
        return QString();
    }

} /* namespace LosStyle */
