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
  explicit GameScene(const QRectF &sceneRect, bool isManualUpdated = false,
                     QObject *parent = nullptr);
  QGraphicsRectItem *getRectItemByEntity(const Entity &entity);
  constexpr static const double SceneScale = 50;

  // костыль для обучения, нужно зарефакторить
  std::tuple<double, bool> getInfoForLearning();

public slots:
  void resetGraphicsScene();
  void pauseAfterUpdate();
  void update();

signals:
  void propagatePressedKey(int key);
  void playerPositionUpdated(QVector<QPointF> newPos);
  void graphicsSceneReseted();
  void pause();
  void unpause();
  void restartGame();

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  QTimer *mUpdateTimer_;
  GameLogic *logic_;
  std::unordered_map<const Entity *, QGraphicsRectItem *> entityToRectItemMap_;
  bool isUpdating_ = false;
  bool isPausedAfterUpdate_ = false;
  bool isManualUpdated_ = false;
};

#endif // GAMESCENE_H
