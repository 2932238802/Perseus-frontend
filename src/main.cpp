#include "Perseus.h"
#include <QApplication>

int main(int argc, char *argv[])
{

#ifdef Q_OS_LINUX
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif
    bool exists = QFile(":/icons/cpp_black.png").exists();
    qDebug() << "/icons/cpp_black.png exists?" << exists;
    QApplication a(argc, argv);
    Perseus w;
    w.show();
    return a.exec();
}
