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
  // double getSceneScale() const;
  constexpr static const double SceneScale = 50;

signals:
  void propagatePressedKey(int key);
  void playerPositionUpdated(QVector<QPointF> newPos);

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  QTimer *mUpdateTimer;
  GameLogic *logic_;
  std::unordered_map<const Entity *, QGraphicsRectItem *> entityToRectItemMap;
  // const double sceneScale = 50;
  bool isUpdating = false;
  // QGraphicsRectItem* rectBox;
  // QGraphicsRectItem* rectGround;

private slots:
  void update();
};

#endif // GAMESCENE_H
