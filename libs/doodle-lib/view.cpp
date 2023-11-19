#include "view.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLWidget>

namespace doodlelib {

View::View(int w, int h, QWidget *widget)
    : QGraphicsView(widget), h_(h), w_(w) {
  viewportRect_.moveCenter(QPointF(50 * GameScene::SceneScale, 0));
  oldCenter_ = viewportRect_.center();

  auto scene = new GameScene(viewportRect_, this);
  setScene(scene);
  resize(w, h);
  scale(1, -1);
  fitInView(viewportRect_);

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
    oldCenter_ = rect.center();
  });
}

void View::renderViewToImage(QImage &renderedImage) {
  QPainter painter(&renderedImage);
  QRect renderRect =
      QRect(QPoint(0, 0), QPoint(this->size().width(), this->size().height()));
  this->render(&painter, renderRect);
}

void View::centerViewOnPlayer(QVector<QPointF> positions) {
  auto playerPos = QPointF(oldCenter_.x(), positions[0].y());
  auto newCenter = oldCenter_ + (playerPos - oldCenter_) / 25;

  if (newCenter.y() <= oldCenter_.y())
    return;

  oldCenter_ = newCenter;

  viewportRect_.moveCenter(newCenter);
  fitInView(viewportRect_);
  setSceneRect(viewportRect_);
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
