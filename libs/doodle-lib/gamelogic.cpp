#include "gamelogic.h"
#include "box2d/box2d.h"
#include "contactlistener.h"
#include "entity.h"
#include "playerentity.h"
#include <QDateTime>
#include <QDebug>

GameLogic *GameLogic::instance_ = nullptr;

GameLogic *GameLogic::GetInstance() {
  if (instance_ == nullptr)
    instance_ = new GameLogic();
  return instance_;
}

void GameLogic::step() {
  // player
  for (auto &it : playerEntityByBody_) {

    // teleport on scene bounds
    auto playerCurrentPos = it.first->GetPosition();
    if (playerCurrentPos.x < sceneBounds[0]) {
      it.first->SetTransform(b2Vec2(sceneBounds[1] - 0.05, playerCurrentPos.y),
                             it.first->GetAngle());
    } else if (it.first->GetPosition().x > sceneBounds[1]) {
      it.first->SetTransform(b2Vec2(sceneBounds[0] + 0.05, playerCurrentPos.y),
                             it.first->GetAngle());
    }

    // move player
    b2Vec2 currentVel = it.first->GetLinearVelocity();
    b2Vec2 desiredVel{};

    bool isJumping = it.second->canJump() && currentVel.y == 0;
    if (isJumping)
      it.second->setLastJumpingTime(QDateTime::currentMSecsSinceEpoch());

    switch (it.second->getCurrentMove()) {
    case MoveType::Left:
      desiredVel.x -= 6;
      desiredVel.y = currentVel.y + (isJumping ? 10 : 0);
      break;
    case MoveType::Right:
      desiredVel.x += 6;
      desiredVel.y = currentVel.y + (isJumping ? 10 : 0);
      break;
    default:
      desiredVel.x = currentVel.x;
      desiredVel.y = currentVel.y + (isJumping ? 10 : 0);
      break;
    }

    if (desiredVel.x == 0 && desiredVel.y == 0)
      continue;

    auto velChange = desiredVel - currentVel;
    auto impulse = it.first->GetMass() * velChange;

    // qDebug() << "deserideVel = " << desiredVel.x << " " << desiredVel.y;
    // qDebug() << "currentVel = " << currentVel.x << " " << currentVel.y;

    it.first->ApplyLinearImpulse(impulse, it.first->GetWorldCenter(), true);

    // it.second->resetCurrentMove();
  }

  // global physics step:
  world_.Step(GameLogic::TimeStep, velocityIterations_, positionIterations_);
}

std::vector<b2Vec2> GameLogic::getPlayerPositions() {
  auto positions = std::vector<b2Vec2>();

  for (auto &pair : playerEntityByBody_) {
    positions.push_back(pair.first->GetPosition());
  }
  return positions;
}

void GameLogic::setSceneHorizontalBounds(double leftBound, double rightBound) {
  sceneBounds[0] = leftBound;
  sceneBounds[1] = rightBound;
}

GameLogic::GameLogic() : QObject(nullptr) {
  world_.SetContactListener(new ContactListener());
}

void GameLogic::propagatePressedKey(int key) {
  for (auto &it : playerEntityByBody_) {
    switch (key) {
    case Qt::Key_D:
      it.second->setCurrentMove(MoveType::Right);
      break;
    case Qt::Key_A:
      it.second->setCurrentMove(MoveType::Left);
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

  if (dynamic_cast<PlayerEntity *>(entity.get())) {
    auto castedPointer = dynamic_cast<PlayerEntity *>(entity.get());
    auto playerEntity = std::shared_ptr<PlayerEntity>(castedPointer);
    playerEntityByBody_[body] = std::move(playerEntity);
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
