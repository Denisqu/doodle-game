#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "box2d/box2d.h"

class Entity;
class EntityRenderer;
class PlayerEntity;

class GameLogic {
public:
  static GameLogic *GetInstance();
  void
  setEntityRenderer(const std::shared_ptr<EntityRenderer> &newEntityRenderer);
  void addEntity(std::unique_ptr<Entity> entity);
  void
  addOnAddEntityCallback(std::function<void(const Entity &entity)> callback);
  void doOnActiveBody(std::function<void(b2Body *body)> func);
  std::shared_ptr<Entity> getEnityByBody(b2Body *body);
  void step();

  static constexpr double TimeStep = 1 / 60.f;

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
};

#endif // GAMELOGIC_H
