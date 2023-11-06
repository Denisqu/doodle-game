#include "view.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLWidget>

namespace doodlelib {

View::View() : QGraphicsView() {
  QRectF rect(QPointF(0, 0), QPointF(720, 720));
  rect.moveCenter(QPointF(50 * GameScene::SceneScale, 0));
  oldCenter = rect.center();

  auto scene = new GameScene(rect, this);
  setScene(scene);
  resize(720, 720);
  scale(1, -1);
  fitInView(rect);

  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  connect(scene, &GameScene::playerPositionUpdated, this,
          &View::centerViewOnPlayer);
}

void View::centerViewOnPlayer(QVector<QPointF> positions) {
  auto playerPos = QPointF(oldCenter.x(), positions[0].y());
  auto newCenter = oldCenter + (playerPos - oldCenter) / 25;

  if (newCenter.y() <= oldCenter.y())
    return;

  oldCenter = newCenter;

  QRectF rect(QPointF(0, 0), QPointF(720, 720));
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
