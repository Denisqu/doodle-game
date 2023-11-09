#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "box2d/box2d.h"
#include "entity.h"

class Entity;
class EntityRenderer;
class PlayerEntity;

class GameLogic : public QObject {
  Q_OBJECT

public:
  static GameLogic *GetInstance();
  void
  setEntityRenderer(const std::shared_ptr<EntityRenderer> &newEntityRenderer);
  b2Body *addEntity(std::unique_ptr<Entity> entity);
  void
  addOnAddEntityCallback(std::function<void(const Entity &entity)> callback);
  void doOnActiveBody(std::function<void(b2Body *body)> func);
  std::shared_ptr<Entity> getEnityByBody(b2Body *body);
  void step();
  std::vector<b2Vec2> getPlayerPositions();
  void setSceneHorizontalBounds(double leftBound, double rightBound);
  void generateObjectPool();
  void updatePlatformPositions();

  static constexpr double TimeStep = 1.0f / 120;
  static constexpr double TimeStepMultiplier = 1.0f;

public slots:
  void propagatePressedKey(int key);

signals:
  void playerLose(std::shared_ptr<PlayerEntity> player);

private:
  explicit GameLogic();
  GameLogic(GameLogic &other) = delete;
  void operator=(const GameLogic &other) = delete;

  static GameLogic *instance_;
  std::shared_ptr<EntityRenderer> entityRenderer_;
  std::unordered_map<b2Body *, std::shared_ptr<Entity>> basicEntityByBody_;
  std::unordered_map<b2Body *, std::shared_ptr<PlayerEntity>>
      playerEntityByBody_;
  std::vector<std::function<void(const Entity &entity)>> onAddEntityCallbacks_;
  const int velocityIterations_ = 8;
  const int positionIterations_ = 3;
  const b2Vec2 gravity_ = b2Vec2(0.0, -10.0f);
  b2World world_ = b2World(gravity_);
  double sceneBounds[2] = {0, 0};
  std::unordered_map<BodyUserData, std::vector<b2Body *>> objectPool;

  friend class ContactListener;
};

Q_DECLARE_METATYPE(std::shared_ptr<PlayerEntity>)

#endif // GAMELOGIC_H
