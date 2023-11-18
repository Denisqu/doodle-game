#include "view.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLWidget>

namespace doodlelib {

View::View(int w, int h, QWidget *widget)
    : QGraphicsView(widget), h_(h), w_(w) {
  QRectF rect(QPointF(0, 0), QPointF(600, 600));
  rect.moveCenter(QPointF(50 * GameScene::SceneScale, 0));
  oldCenter = rect.center();

  auto scene = new GameScene(rect, this);
  setScene(scene);
  resize(w, h);
  scale(1, -1);
  fitInView(rect);

  setDragMode(QGraphicsView::NoDrag);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  connect(this, &View::pauseAfterUpdate, scene, &GameScene::pauseAfterUpdate);
  connect(this, &View::pause, scene, &GameScene::pause);
  connect(this, &View::unpause, scene, &GameScene::unpause);
  connect(this, &View::restartGame, scene, &GameScene::restartGame);
  connect(scene, &GameScene::playerPositionUpdated, this,
          &View::centerViewOnPlayer);
  connect(scene, &GameScene::graphicsSceneReseted, this, [this] {
    QRectF rect(QPointF(0, 0), QPointF(w_, h_));
    rect.moveCenter(QPointF(50 * GameScene::SceneScale, 0));
    oldCenter = rect.center();
  });
}

void View::centerViewOnPlayer(QVector<QPointF> positions) {
  auto playerPos = QPointF(oldCenter.x(), positions[0].y());
  auto newCenter = oldCenter + (playerPos - oldCenter) / 25;

  if (newCenter.y() <= oldCenter.y())
    return;

  oldCenter = newCenter;

  QRectF rect(QPointF(0, 0), QPointF(600, 600));
  rect.moveCenter(newCenter);
  fitInView(rect);
  setSceneRect(rect);
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
