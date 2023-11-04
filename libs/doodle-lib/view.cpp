#include "view.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

namespace doodlelib {

View::View() : QGraphicsView() {
  auto scene = new GameScene(this);
  setScene(scene);
  resize(1280, 720);
  scale(1, -1);

  QRectF rect(QPointF(0, 0), QPointF(3000, 1000));
  oldCenter = rect.center();
  fitInView(rect);
  setSceneRect(rect);

  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  connect(scene, &GameScene::playerPositionUpdated, this,
          &View::centerViewOnPlayer);
}

void View::centerViewOnPlayer(QVector<QPointF> positions) {
  auto playerPos = positions[0];
  auto newCenter = oldCenter + (playerPos - oldCenter) / 25;
  oldCenter = newCenter;

  QRectF rect(QPointF(0, 0), QPointF(3000, 1000));
  rect.moveCenter(newCenter);
  fitInView(rect);
  setSceneRect(rect);

  qDebug() << "Centering View on Player!" << positions[0];
}

/**
 * Exit application on Escape button
 */
void View::keyPressEvent(QKeyEvent *event) {
  if (!event->isAutoRepeat()) {
    switch (event->key()) {
    case Qt::Key_Escape:
      QApplication::instance()->quit();
      break;
    }
  }
  QGraphicsView::keyPressEvent(event);
}

} // namespace doodlelib
