#include "view.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

namespace doodlelib {

View::View() : QGraphicsView() {
  auto scene = new GameScene(this);
  setScene(scene);
  connect(scene, &GameScene::playerPositionUpdated, this,
          &View::centerViewOnPlayer);

  resize(1280, 720);
  scale(1, -1);
  setDragMode(QGraphicsView::NoDrag);
  // QRectF rect(QPointF(0, 0), QPointF(3000, 1000));
  // fitInView(rect);
  // setSceneRect(rect);

  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::centerViewOnPlayer(QVector<QPointF> positions) {
  centerOn(positions[0]);

  QRectF rect(QPointF(0, 0), QPointF(3000, 1000));
  rect.moveCenter(positions[0]);
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
