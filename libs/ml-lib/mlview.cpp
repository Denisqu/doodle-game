#include "mlview.h"
#include "gamelogic.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>

MLView::MLView() : renderToImageTimer{} {
  int argc = 1;
  char *argv = "1";
  QApplication a(argc, &argv);

  auto gameScene = static_cast<GameScene *>(scene());
  double renderToImageInterval =
      GameLogic::TimeStep * GameLogic::TimeStepMultiplier;
  renderToImageTimer.setInterval(renderToImageInterval * 1000);

  a.exec();
}
