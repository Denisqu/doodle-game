#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <unordered_map>

#include "box2d/box2d.h"

class GameLogic;
class Entity;

class GameScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit GameScene(QObject *parent = nullptr);
  QGraphicsRectItem *getRectItemByEntity(const Entity &entity);

signals:
  void propagatePressedKey(int key);

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  QTimer *mUpdateTimer;
  GameLogic *logic_;
  std::unordered_map<const Entity *, QGraphicsRectItem *> entityToRectItemMap;
  const double sceneScale = 50;
  // QGraphicsRectItem* rectBox;
  // QGraphicsRectItem* rectGround;

private slots:
  void update();
};

#endif // GAMESCENE_H
