#include <QApplication>
#include "mainwindow.h"
//#define plugins
#ifdef plugins
	#include "./plugin/qonsole.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

#ifdef plugins
    Qonsole w2;
    w2.show();
#endif 

    return a.exec();
}
