#include "gamelogic.h"
#include "box2d/box2d.h"
#include "entity.h"
#include "playerentity.h"

GameLogic *GameLogic::instance_ = nullptr;

GameLogic *GameLogic::GetInstance() {
  if (instance_ == nullptr)
    instance_ = new GameLogic();
  return instance_;
}

void GameLogic::step() {
  // move player:

  world_.Step(GameLogic::TimeStep, velocityIterations_, positionIterations_);
}

GameLogic::GameLogic() {}

std::shared_ptr<Entity> GameLogic::getEnityByBody(b2Body *body) {
  // return std::make_shared<Entity>(entityByBody_[body]);
  return std::shared_ptr<Entity>(basicEntityByBody_[body]);
}

// TODO: возможно нужно будет удалить
void GameLogic::setEntityRenderer(
    const std::shared_ptr<EntityRenderer> &newEntityRenderer) {
  entityRenderer_ = newEntityRenderer;
}

void GameLogic::addEntity(std::unique_ptr<Entity> entity) {
  for (const auto &callback : onAddEntityCallbacks_) {
    callback(*entity);
  }

  b2Body *body = world_.CreateBody(&entity->bodyDef());
  body->CreateFixture(&entity->fixtureDef());

  if (dynamic_cast<PlayerEntity *>(entity.get())) {
    auto castedPointer = dynamic_cast<PlayerEntity *>(entity.release());
    auto playerEntity = std::unique_ptr<PlayerEntity>(castedPointer);

    playerEntityByBody_[body] = std::move(playerEntity);
    return;
  }

  basicEntityByBody_[body] = std::move(entity);
}

void GameLogic::addOnAddEntityCallback(
    std::function<void(const Entity &)> callback) {
  onAddEntityCallbacks_.push_back(callback);
}

void GameLogic::doOnActiveBody(std::function<void(b2Body *)> func) {
  for (b2Body *bIter = world_.GetBodyList(); bIter != 0;
       bIter = bIter->GetNext()) {
    func(bIter);
  }
}
