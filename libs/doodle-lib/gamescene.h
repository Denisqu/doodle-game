#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QTimer>
#include <unordered_map>

#include "box2d/box2d.h"

class GameLogic;
class Entity;

class GameScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit GameScene(const QRectF &sceneRect, QObject *parent = nullptr);
  QGraphicsRectItem *getRectItemByEntity(const Entity &entity);
  constexpr static const double SceneScale = 50;

  void pauseAfterUpdate();

public slots:
  void resetGraphicsScene();

signals:
  void propagatePressedKey(int key);
  void playerPositionUpdated(QVector<QPointF> newPos);
  void graphicsSceneReseted();
  void pause();
  void unpause();

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  QTimer *mUpdateTimer;
  GameLogic *logic_;
  std::unordered_map<const Entity *, QGraphicsRectItem *> entityToRectItemMap;
  bool isUpdating = false;
  bool isPausedAfterUpdate = false;

private slots:
  void update();
};

#endif // GAMESCENE_H
