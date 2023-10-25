#include "mainwindow.h"
#include "libs/doodle-lib/doodlelib.h"
#include <QDebug>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "getOne = " << DoodleLib().getOne();
    return a.exec();
}
