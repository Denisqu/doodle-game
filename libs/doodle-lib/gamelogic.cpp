#include "gamelogic.h"
#include "box2d/box2d.h"
#include "entity.h"

GameLogic *GameLogic::GetInstance() {
  if (instance_ == nullptr)
    instance_ = new GameLogic();
  return instance_;
}

void GameLogic::step() {
  world_.Step(GameLogic::TimeStep, velocityIterations_, positionIterations_);
}

GameLogic::GameLogic() {}

std::shared_ptr<Entity> GameLogic::getEnityByBody(b2Body *body) {
  return std::make_shared<Entity>(entityByBody_[*body]);
}

// TODO: возможно нужно будет удалить
void GameLogic::setEntityRenderer(
    const std::shared_ptr<EntityRenderer> &newEntityRenderer) {
  entityRenderer_ = newEntityRenderer;
}

void GameLogic::addEntity(std::unique_ptr<Entity> entity) {
  for (auto callback : onAddEntityCallbacks_) {
    callback(*entity);
  }

  b2Body *body = world_.CreateBody(&entity->bodyDef());
  body->CreateFixture(&entity->fixtureDef());

  entityByBody_[*body] = std::move(entity);
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
