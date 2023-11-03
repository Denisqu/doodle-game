#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "entity.h"
#include <QDateTime>
#include <QObject>

enum class MoveType { NoOp, Right, Left };
enum class ControllerType { Human, NeuralNetwork };

class PlayerEntity : public Entity {
public:
  PlayerEntity(std::unique_ptr<EntityPhysicsInfo> physicsInfo,
               ControllerType controllerType);

  void setCurrentMove(MoveType type);
  const MoveType &getCurrentMove() const;
  void resetCurrentMove();
  void increaseFootContacts();
  void decreaseFootContacts();
  int getFootContactsCount();
  qint64 getLastJumpingTime() const;
  void setLastJumpingTime(qint64 newLastJumpingTime);
  qint64 getJumpTimeoutMs() const;
  void setJumpTimeoutMs(qint64 newJumpTimeoutMs);
  bool canJump();

private:
  ControllerType controllerType_;
  MoveType currentMove_ = MoveType::NoOp;
  unsigned int footContactsCount_ = 0;
  qint64 lastJumpingTimeMs = QDateTime::currentMSecsSinceEpoch();
  qint64 jumpTimeoutMs = 100;
};

#endif // PLAYERENTITY_H
