#include "playerentity.h"

PlayerEntity::PlayerEntity(std::unique_ptr<EntityPhysicsInfo> physicsInfo,
                           ControllerType controllerType)
    : Entity(std::move(physicsInfo)), controllerType_(controllerType) {
  physicsInfo_->bodyUserData = BodyUserData::Player;
}

void PlayerEntity::setCurrentMove(MoveType type) { currentMove_ = type; }

const MoveType &PlayerEntity::getCurrentMove() const { return currentMove_; }

void PlayerEntity::resetCurrentMove() { currentMove_ = MoveType::NoOp; }

void PlayerEntity::increaseFootContacts() { ++footContactsCount_; }

void PlayerEntity::decreaseFootContacts() { --footContactsCount_; }

int PlayerEntity::getFootContactsCount() { return footContactsCount_; }

qint64 PlayerEntity::getLastJumpingTime() const { return lastJumpingTimeMs; }

void PlayerEntity::setLastJumpingTime(qint64 newLastJumpingTime) {
  lastJumpingTimeMs = newLastJumpingTime;
}

qint64 PlayerEntity::getJumpTimeoutMs() const { return jumpTimeoutMs; }

void PlayerEntity::setJumpTimeoutMs(qint64 newJumpTimeoutMs) {
  jumpTimeoutMs = newJumpTimeoutMs;
}

bool PlayerEntity::canJump() {
  bool isContactingGround = getFootContactsCount() != 0;
  bool isTimeoutDone = (QDateTime::currentMSecsSinceEpoch() - jumpTimeoutMs) >=
                       lastJumpingTimeMs;
  return isContactingGround && isTimeoutDone;
}

/*
 * Reference:
 *
 * b2Vec2 vel = body->GetLinearVelocity();
float desiredVel = 0;
switch ( moveState )
{
  case MS_LEFT:  desiredVel = -5; break;
  case MS_STOP:  desiredVel =  0; break;
  case MS_RIGHT: desiredVel =  5; break;
}
float velChange = desiredVel - vel.x;
float impulse = body->GetMass() * velChange; //disregard time factor
body->ApplyLinearImpulse( b2Vec2(impulse,0), body->GetWorldCenter() );
 *
 *
 */
