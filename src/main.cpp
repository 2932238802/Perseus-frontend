#include "Perseus.h"
#include <QApplication>
#include <qcoreapplication.h>
#include <qglobal.h>

int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif
    QApplication a(argc, argv);
    Perseus w;
    w.show();
    return a.exec();
}
