#include "playerentity.h"

PlayerEntity::PlayerEntity(std::unique_ptr<EntityPhysicsInfo> physicsInfo,
                           ControllerType controllerType)
    : Entity(std::move(physicsInfo)), controllerType_(controllerType) {
  physicsInfo_->entityType = BodyUserData::Player;
}

void PlayerEntity::setCurrentMove(MoveType type) { currentMove_ = type; }

const MoveType &PlayerEntity::getCurrentMove() const { return currentMove_; }

void PlayerEntity::resetCurrentMove() { currentMove_ = MoveType::NoOp; }

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