#include "entityconstructor.h"
#include "entity.h"
#include "playerentity.h"
#include <memory>

namespace {

std::tuple<
    std::pair<b2BodyDef, BodyUserData>,
    std::vector<std::tuple<b2FixtureDef, b2PolygonShape, FixtureUserData>>>
createBase(b2Vec2 dims, b2Vec2 pos) {
  auto bodyDef = b2BodyDef();
  bodyDef.position = pos;
  bodyDef.type = b2_staticBody;
  bodyDef.fixedRotation = true;
  auto bodyUserData = BodyUserData::Default;

  auto fixtureTuple =
      std::vector<std::tuple<b2FixtureDef, b2PolygonShape, FixtureUserData>>();

  auto mainShape = b2PolygonShape();
  mainShape.SetAsBox(dims.x / 2.f, dims.y / 2.f);
  auto mainFixtureDef = b2FixtureDef();
  mainFixtureDef.density = 1.0f;
  mainFixtureDef.friction = 0.7f;
  fixtureTuple.push_back({mainFixtureDef, mainShape, FixtureUserData::Default});

  return {{bodyDef, bodyUserData}, fixtureTuple};
}

} // namespace

Entity *EntityConstructor::CreateStaticBox(b2Vec2 dims, b2Vec2 pos) {
  auto base = createBase(dims, pos);
  auto bodyDef = std::move(std::get<0>(base));
  auto fixtureShapeDefs = std::move(std::get<1>(base));

  auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, std::move(bodyDef), std::move(fixtureShapeDefs));
  auto entity = new Entity(std::move(physicsInfo));

  return entity;
}

Entity *EntityConstructor::CreateDynamicBox(b2Vec2 dims, b2Vec2 pos) {
  auto base = createBase(dims, pos);
  auto bodyDef = std::move(std::get<0>(base));
  bodyDef.first.type = b2_dynamicBody;
  auto fixtureShapeDefs = std::move(std::get<1>(base));

  auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, std::move(bodyDef), std::move(fixtureShapeDefs));
  auto entity = new Entity(std::move(physicsInfo));

  return entity;
}

PlayerEntity *
EntityConstructor::CreatePlayerEntity(b2Vec2 dims, b2Vec2 pos,
                                      ControllerType controllerType) {
  auto base = createBase(dims, pos);
  std::get<0>(base).second = BodyUserData::Player;
  auto bodyDef = std::move(std::get<0>(base));
  bodyDef.first.type = b2_dynamicBody;
  auto fixtureShapeDefs = std::move(std::get<1>(base));

  // TODO: ADD FIXTURE DEF FOR FOOT SENSOR

  auto physicsInfo = std::make_unique<EntityPhysicsInfo>(
      dims, std::move(bodyDef), std::move(fixtureShapeDefs));
  auto entity = new PlayerEntity(std::move(physicsInfo), controllerType);

  return entity;
}

EntityConstructor::EntityConstructor() {}
