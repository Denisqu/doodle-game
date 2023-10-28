#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "entity.h"

enum class MoveType { Right, Left, Jump };
enum class ControllerType { Human, NeuralNetwork };

class PlayerEntity : public Entity {
public:
  PlayerEntity(std::unique_ptr<b2BodyDef> bodyDef,
               std::unique_ptr<b2FixtureDef> fixtureDef, b2Vec2 boxDims);

  void move(MoveType mType);

private:
};

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

#endif // PLAYERENTITY_H
