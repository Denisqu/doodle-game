#ifndef ENTITYCONSTRUCTOR_H
#define ENTITYCONSTRUCTOR_H

#include "box2d/box2d.h"
class Entity;
class PlayerEntity;
enum class ControllerType;

class EntityConstructor {
public:
  static Entity *CreateStaticBox(b2Vec2 dims, b2Vec2 pos);
  static Entity *CreateDynamicBox(b2Vec2 dims, b2Vec2 pos);
  static PlayerEntity *CreatePlayerEntity(b2Vec2 dims, b2Vec2 pos,
                                          ControllerType contorllerType);

private:
  EntityConstructor();
};

#endif // ENTITYCONSTRUCTOR_H
