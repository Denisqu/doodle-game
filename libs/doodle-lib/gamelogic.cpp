#include "gamelogic.h"
#include "box2d/box2d.h"
#include "contactlistener.h"
#include "entity.h"
#include "entityconstructor.h"
#include "playerentity.h"
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

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

    // lose player
    if (it.first->GetLinearVelocity().y < -15) {
      qDebug() << "You lose!";
      emit playerLose(it.second);
    }

    // update reward
    if (it.first->GetLinearVelocity().y > 0 &&
        it.first->GetPosition().y > it.second->getReward()) {
      it.second->setReward(it.first->GetPosition().y);
      qDebug() << it.second->getReward();
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

    it.first->ApplyLinearImpulse(impulse, it.first->GetWorldCenter(), true);
  }

  // move platforms
  static b2Vec2 previousPlatformUpdatePos{
      playerEntityByBody_.begin()->first->GetPosition()};

  if (playerEntityByBody_.begin()->first->GetPosition().y -
          previousPlatformUpdatePos.y >
      3) {
    updatePlatformPositions();
    previousPlatformUpdatePos =
        playerEntityByBody_.begin()->first->GetPosition();
  }

  // global physics step:
  world_.Step(GameLogic::TimeStep * GameLogic::TimeStepMultiplier,
              velocityIterations_, positionIterations_);
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

void GameLogic::generateObjectPool() {
  objectPool[BodyUserData::Platform] = {};
  for (int i = 0; i < 10; ++i) {
    auto platformBox =
        std::unique_ptr<Entity>(EntityConstructor::CreateStaticBox(
            b2Vec2(3.0f, 0.25f), b2Vec2(100, 100)));
    auto platformBody = this->addEntity(std::move(platformBox));
    objectPool[BodyUserData::Platform].push_back(platformBody);
  }
}

void GameLogic::updatePlatformPositions() {
  static unsigned int nextPlatformIndex{};
  static b2Vec2 lastPlatformPosition =
      playerEntityByBody_.begin()->first->GetPosition();

  // 1) get next platform from object pool
  nextPlatformIndex =
      nextPlatformIndex % objectPool[BodyUserData::Platform].size();
  auto nextPlatform = objectPool[BodyUserData::Platform][nextPlatformIndex];
  nextPlatformIndex++;

  // 2) calculate next platform position and verify position
  bool isInSceneBounds = false;
  auto nextPos = b2Vec2(-999, -999);
  while (!isInSceneBounds) {
    auto xOffset = QRandomGenerator::global()->generateDouble() * 20 - 10;
    nextPos =
        b2Vec2(lastPlatformPosition.x + xOffset, lastPlatformPosition.y + 3);
    isInSceneBounds = nextPos.x > sceneBounds[0] && nextPos.x < sceneBounds[1];
  }
  lastPlatformPosition = nextPos;

  // 3) update next platform position
  nextPlatform->SetTransform(nextPos, nextPlatform->GetAngle());
}

GameLogic::GameLogic() : QObject(nullptr) {
  qRegisterMetaType<std::shared_ptr<PlayerEntity>>();
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

b2Body *GameLogic::addEntity(std::unique_ptr<Entity> entity) {
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

  return body;
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
