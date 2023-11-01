#include "entityconstructor.h"
#include "entity.h"
#include "playerentity.h"
#include <memory>

namespace {

std::tuple<BodyUserData, std::unique_ptr<b2BodyDef>,
           std::vector<std::pair<b2FixtureDef, b2PolygonShape>>>
createBase(b2Vec2 dims, b2Vec2 pos) {
  auto bodyDef = std::make_unique<b2BodyDef>();
  bodyDef->position = pos;
  bodyDef->type = b2_staticBody;
  bodyDef->fixedRotation = true;

  auto bodyUserData = BodyUserData::Platform;

  auto fixtureShapeDefs =
      std::vector<std::pair<b2FixtureDef, b2PolygonShape>>();

  auto mainShape = b2PolygonShape();
  mainShape.SetAsBox(dims.x / 2.f, dims.y / 2.f);
  auto mainFixtureDef = b2FixtureDef();
  mainFixtureDef.density = 1.0f;
  mainFixtureDef.friction = 0.7f;
  fixtureShapeDefs.push_back({mainFixtureDef, mainShape});

  return {bodyUserData, std::move(bodyDef), fixtureShapeDefs};
}

} // namespace

Entity *EntityConstructor::CreateStaticBox(b2Vec2 dims, b2Vec2 pos) {
  auto base = createBase(dims, pos);
  auto bodyDef = std::move(std::get<1>(base));
  auto bodyUserData = std::move(std::get<0>(base));
  auto fixtureShapeDefs = std::move(std::get<2>(base));

  auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, bodyUserData, std::move(bodyDef), std::move(fixtureShapeDefs));
  auto entity = new Entity(std::move(physicsInfo));

  return entity;
}

Entity *EntityConstructor::CreateDynamicBox(b2Vec2 dims, b2Vec2 pos) {
  auto base = createBase(dims, pos);
  auto bodyDef = std::move(std::get<1>(base));
  bodyDef->type = b2_dynamicBody;
  auto bodyUserData = std::move(std::get<0>(base));
  auto fixtureShapeDefs = std::move(std::get<2>(base));

  auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, bodyUserData, std::move(bodyDef), std::move(fixtureShapeDefs));
  auto entity = new Entity(std::move(physicsInfo));

  return entity;
}

PlayerEntity *
EntityConstructor::CreatePlayerEntity(b2Vec2 dims, b2Vec2 pos,
                                      ControllerType controllerType) {
  /*auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, bodyUserData, std::move(bodyDef), std::move(fixtureShapeDefs));

  auto entity = new PlayerEntity(std::move(physicsInfo), ControllerType::Human);

  return entity;
  */
  auto base = createBase(dims, pos);
  auto bodyDef = std::move(std::get<1>(base));
  bodyDef->type = b2_dynamicBody;
  auto bodyUserData = std::move(std::get<0>(base));
  auto fixtureShapeDefs = std::move(std::get<2>(base));

  auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, bodyUserData, std::move(bodyDef), std::move(fixtureShapeDefs));
  auto entity = new PlayerEntity(std::move(physicsInfo), controllerType);

  return entity;
}

EntityConstructor::EntityConstructor() {}
