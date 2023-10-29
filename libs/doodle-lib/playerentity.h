#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "entity.h"

enum class MoveType { NoOp, Right, Left, Jump };
enum class ControllerType { Human, NeuralNetwork };

class PlayerEntity : public Entity {
public:
  PlayerEntity(std::unique_ptr<EntityPhysicsInfo> physicsInfo,
               ControllerType controllerType);

  void setCurrentMove(MoveType type);
  const MoveType &getCurrentMove() const;
  void resetCurrentMove();

private:
  ControllerType controllerType_;
  MoveType currentMove_ = MoveType::NoOp;
};

#endif // PLAYERENTITY_H
