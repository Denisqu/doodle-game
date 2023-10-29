#include "entityconstructor.h"
#include "entity.h"
#include <memory>

Entity *EntityConstructor::CreateStaticBox(b2Vec2 dims, b2Vec2 pos) {
  auto physicsInfo =
      std::make_unique<EntityPhysicsInfo>(dims, pos, b2_staticBody, 1.f, 0.7f);

  auto entity = new Entity(std::move(physicsInfo));

  return entity;
}

Entity *EntityConstructor::CreateDynamicBox(b2Vec2 dims, b2Vec2 pos) {
  auto physicsInfo =
      std::make_unique<EntityPhysicsInfo>(dims, pos, b2_dynamicBody, 1.f, 0.7f);

  auto entity = new Entity(std::move(physicsInfo));

  return entity;
}

EntityConstructor::EntityConstructor() {}
