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

GameLogic::GameLogic()
    : QObject(nullptr), contactListener_(new ContactListener()) {
  qRegisterMetaType<PlayerEntity *>();
    connect(this, &GameLogic::playerLose, this, &GameLogic::restartGame);
}

GameState GameLogic::state() const
{
    return state_;
}

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
    if (playerCurrentPos.x < sceneBounds_[0]) {
      it.first->SetTransform(b2Vec2(sceneBounds_[1] - 0.05, playerCurrentPos.y),
                             it.first->GetAngle());
    } else if (it.first->GetPosition().x > sceneBounds_[1]) {
      it.first->SetTransform(b2Vec2(sceneBounds_[0] + 0.05, playerCurrentPos.y),
                             it.first->GetAngle());
    }

    // lose player
    if (it.first->GetLinearVelocity().y < -14) {
      qDebug() << "You lose!";
      emit playerLose(it.second.get());
      return;
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
  if (playerEntityByBody_.begin()->first->GetPosition().y -
          previousPlatformUpdatePos_.y >
      3) {
    updatePlatformPositions();
    previousPlatformUpdatePos_ =
        playerEntityByBody_.begin()->first->GetPosition();
  }

  // global physics step:
  world_->Step(GameLogic::TimeStep * GameLogic::TimeStepMultiplier,
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
  sceneBounds_[0] = leftBound;
  sceneBounds_[1] = rightBound;
}

void GameLogic::generateObjectPool() {
  objectPool_[BodyUserData::Platform] = {};
  for (int i = 0; i < 10; ++i) {
    auto platformBox =
        std::unique_ptr<Entity>(EntityConstructor::CreateStaticBox(
            b2Vec2(3.0f, 0.25f), b2Vec2(100, 100)));
    auto platformBody = this->addEntity(std::move(platformBox));
    objectPool_[BodyUserData::Platform].push_back(platformBody);
  }
}

void GameLogic::updatePlatformPositions(PlatformGenerationState state) {
  static unsigned int nextPlatformIndex{};
  static b2Vec2 lastPlatformPosition =
      playerEntityByBody_.begin()->first->GetPosition();

  if (state == PlatformGenerationState::Reset) {
    nextPlatformIndex = 0;
    lastPlatformPosition = playerEntityByBody_.begin()->first->GetPosition();
  }

  // 1) get next platform from object pool
  nextPlatformIndex =
      nextPlatformIndex % objectPool_[BodyUserData::Platform].size();
  auto nextPlatform = objectPool_[BodyUserData::Platform][nextPlatformIndex];
  nextPlatformIndex++;

  // 2) calculate next platform position and verify position
  bool isInSceneBounds = false;
  auto nextPos = b2Vec2(-999, -999);
  while (!isInSceneBounds) {
    auto xOffset = QRandomGenerator::global()->generateDouble() * 20 - 10;
    nextPos =
        b2Vec2(lastPlatformPosition.x + xOffset, lastPlatformPosition.y + 3);
    isInSceneBounds =
        nextPos.x > sceneBounds_[0] && nextPos.x < sceneBounds_[1];
  }
  lastPlatformPosition = nextPos;

  // 3) update next platform position
  nextPlatform->SetTransform(nextPos, nextPlatform->GetAngle());
}

void GameLogic::startGame() {
  if (state_ == GameState::Started)
    return;

  world_ = std::make_unique<b2World>(gravity_);
  world_->SetContactListener(contactListener_.get());
  generateObjectPool();
  auto groundBox = std::unique_ptr<Entity>(
      EntityConstructor::CreateStaticBox(b2Vec2(100.0f, 2.0f), b2Vec2(50, 0)));
  addEntity(std::move(groundBox));
  auto playerEntity = std::unique_ptr<Entity>(
      static_cast<Entity *>(EntityConstructor::CreatePlayerEntity(
          b2Vec2(0.5f, 0.5f), b2Vec2(50, 1), ControllerType::Human)));
  addEntity(std::move(playerEntity));

  updatePlatformPositions(PlatformGenerationState::Reset);
  for (int i = 0; i < 2; ++i) {
    updatePlatformPositions();
  }

  state_ = GameState::Started;
}

void GameLogic::restartGame() {
  if (state_ == GameState::Invalid)
    return;

  emit gameRestartStarted();
  // reset data structures
  state_ = GameState::Invalid;
  basicEntityByBody_ = std::unordered_map<b2Body *, std::shared_ptr<Entity>>();
  playerEntityByBody_ =
      std::unordered_map<b2Body *, std::shared_ptr<PlayerEntity>>();
  objectPool_ = std::unordered_map<BodyUserData, std::vector<b2Body *>>();
  previousPlatformUpdatePos_ = {0, 0};
  world_.reset();

  startGame();

  emit gameRestartEnded();
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

void GameLogic::pause() { state_ = GameState::Pause; }

void GameLogic::unpause() { state_ = GameState::Started; }

std::shared_ptr<Entity> GameLogic::getEnityByBody(b2Body *body) {
  return std::shared_ptr<Entity>(basicEntityByBody_[body]);
}

b2Body *GameLogic::addEntity(std::unique_ptr<Entity> entity) {
  for (const auto &callback : onAddEntityCallbacks_) {
    callback(*entity);
  }

  b2Body *body = world_->CreateBody(&entity->physicsInfo().bodyDefPair.first);
  for (const auto &tuple : entity->physicsInfo().fixtureShapeDefs) {
    body->CreateFixture(&std::get<0>(tuple));
  }

  basicEntityByBody_[body] = std::move(entity);

  if (std::dynamic_pointer_cast<PlayerEntity>(basicEntityByBody_[body])) {
    // auto castedPointer = dynamic_cast<PlayerEntity *>(entity.get());
    auto castedPointer =
        std::dynamic_pointer_cast<PlayerEntity>(basicEntityByBody_[body]);
    auto playerEntity = std::shared_ptr<PlayerEntity>(castedPointer);
    playerEntityByBody_[body] = std::move(playerEntity);
  }

  return body;
}

void GameLogic::addOnAddEntityCallback(
    std::function<void(const Entity &)> callback) {
  onAddEntityCallbacks_.push_back(callback);
}

void GameLogic::doOnActiveBody(std::function<void(b2Body *)> func) {
  for (b2Body *bIter = world_->GetBodyList(); bIter != 0;
       bIter = bIter->GetNext()) {
    func(bIter);
  }
}
