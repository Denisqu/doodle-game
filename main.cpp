#include <QDebug>
#include <QApplication>

#include "mainwindow.h"
#include "libs/doodle-lib/doodlelib.h"
#include "libs/doodle-lib/view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    doodlelib::View view;
    DoodleLib().helloWorldBox2D();
    view.show();
    return a.exec();
}
