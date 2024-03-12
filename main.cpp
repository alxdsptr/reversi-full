#include "mainwindow.h"
//#include "fullwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    BoardWidget b;
//    b.show();
    MainWindow w;
    w.show();

    return a.exec();
}
