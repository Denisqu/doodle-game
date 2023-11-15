#include <QApplication>
#include <QDebug>
#include <QThread>

#include "libs/doodle-lib/doodlelib.h"
#include "libs/doodle-lib/view.h"
#include "mainwindow.h"
#include "mlclient.h"
#include "mlserver.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  constexpr bool isLearning = true;

  if constexpr (isLearning) {
    // сервер должен сидеть не в main потоке
    auto serverThread = new QThread(&a);
    auto server = new MLServer(80);
    server->moveToThread(serverThread);
    serverThread->start();

  } else {
    doodlelib::View view;
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setRenderHints(QPainter::Antialiasing);
    view.show();
  }

  return a.exec();
}
