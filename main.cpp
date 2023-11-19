#include <QApplication>
#include <QDebug>
#include <QThread>

#include "doodleapplication.h"
#include "doodleenv.h"
#include "libs/doodle-lib/doodlelib.h"
#include "libs/doodle-lib/view.h"
#include "mainwindow.h"
#include "mlclient.h"
#include "mlserver.h"
#include "mlview.h"

int main(int argc, char *argv[]) {
  DoodleApplication a(argc, argv);

  constexpr bool isLearning = true;

  if constexpr (isLearning) {
    auto env = new DoodleEnv(&a);
    // сервер должен сидеть не в main потоке
    a.serverThread = new QThread(&a);
    auto server = new MLServer(80);
    server->moveToThread(a.serverThread);

    QObject::connect(server, &MLServer::make, env, &DoodleEnv::make);
    QObject::connect(server, &MLServer::reset, env, &DoodleEnv::reset);
    QObject::connect(server, &MLServer::step, env, &DoodleEnv::step);
    QObject::connect(env, &DoodleEnv::makeEnd, server, &MLServer::makeCallback);
    QObject::connect(env, &DoodleEnv::resetEnd, server,
                     &MLServer::resetCallback);
    QObject::connect(env, &DoodleEnv::stepEnd, server, &MLServer::stepCallback);

    a.serverThread->start();
  } else {
    doodlelib::View view;
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setRenderHints(QPainter::Antialiasing);
    view.show();
  }

  return a.exec();
}
