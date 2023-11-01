#include "gamelogic.h"
#include "box2d/box2d.h"
#include "entity.h"
#include "playerentity.h"
#include <QDebug>

GameLogic *GameLogic::instance_ = nullptr;

GameLogic *GameLogic::GetInstance() {
  if (instance_ == nullptr)
    instance_ = new GameLogic();
  return instance_;
}

void GameLogic::step() {
  // move player:
  for (auto &it : playerEntityByBody_) {
    if (it.second->getCurrentMove() == MoveType::NoOp)
      continue;

    b2Vec2 currentVel = it.first->GetLinearVelocity();
    b2Vec2 desiredVel{};

    switch (it.second->getCurrentMove()) {
    case MoveType::Jump:
      desiredVel.y += 5;
      desiredVel.x = currentVel.x;
      break;
    case MoveType::Left:
      desiredVel.x -= 3;
      desiredVel.y = currentVel.y;
      break;
    case MoveType::Right:
      desiredVel.x += 3;
      desiredVel.y = currentVel.y;
      break;
    default:
      break;
    }
    auto velChange = desiredVel - currentVel;
    auto impulse = it.first->GetMass() * velChange;
    it.first->ApplyLinearImpulse(impulse, it.first->GetWorldCenter(), true);

    it.second->resetCurrentMove();
  }

  // global physics step:
  world_.Step(GameLogic::TimeStep, velocityIterations_, positionIterations_);
}

GameLogic::GameLogic() : QObject(nullptr) {}

void GameLogic::propagatePressedKey(int key) {
  for (auto &it : playerEntityByBody_) {
    switch (key) {
    case Qt::Key_D:
      it.second->setCurrentMove(MoveType::Right);
      break;
    case Qt::Key_A:
      it.second->setCurrentMove(MoveType::Left);
      break;
    case Qt::Key_Space:
      it.second->setCurrentMove(MoveType::Jump);
      break;
    default:
      break;
    }
  }
}

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

  b2Body *body = world_.CreateBody(&entity->physicsInfo().bodyDefPair.first);
  for (const auto &tuple : entity->physicsInfo().fixtureShapeDefs) {
    body->CreateFixture(&std::get<0>(tuple));
  }
  basicEntityByBody_[body] = std::move(entity);

  if (dynamic_cast<PlayerEntity *>(entity.get())) {
    auto castedPointer = dynamic_cast<PlayerEntity *>(entity.get());
    auto playerEntity = std::shared_ptr<PlayerEntity>(castedPointer);
    playerEntityByBody_[body] = std::move(playerEntity);
  }
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
