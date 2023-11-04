#include <QApplication>
#include <QDebug>

#include "libs/doodle-lib/doodlelib.h"
#include "libs/doodle-lib/view.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  doodlelib::View view;
  view.show();
  return a.exec();
}
