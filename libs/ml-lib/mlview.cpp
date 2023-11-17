#include "mlview.h"
#include "gamelogic.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>

MLView::MLView(QWidget *parent)
    : doodlelib::View(parent), renderToImageTimer{} {
  auto gameScene = static_cast<GameScene *>(scene());
  double renderToImageInterval =
      GameLogic::TimeStep * GameLogic::TimeStepMultiplier;
  renderToImageTimer.setInterval(renderToImageInterval * 1000);
}
