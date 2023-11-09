#include <QApplication>
#include <QDebug>

#include "libs/doodle-lib/doodlelib.h"
#include "libs/doodle-lib/view.h"
#include "mainwindow.h"
#include "mllib.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  /*
  doodlelib::View view;
  view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  view.setRenderHints(QPainter::Antialiasing);
  view.show();
  */
  qDebug() << ml::MLLib().getTestStringFromTorch().c_str();

  return a.exec();
}
