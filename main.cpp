#include <QApplication>
#include <QDebug>

#include "libs/doodle-lib/doodlelib.h"
#include "libs/doodle-lib/view.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  doodlelib::View view;
  view.setDragMode(QGraphicsView::NoDrag);
  QRectF rect(QPointF(0, 0), QPointF(3000, 1000));
  view.fitInView(rect);
  view.setSceneRect(rect);
  view.show();
  return a.exec();
}
