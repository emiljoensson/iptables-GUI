#include "qonsole.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qonsole w;
    w.show();

    return a.exec();
}
