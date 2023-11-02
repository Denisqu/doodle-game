#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <vector>

#include "box2d/box2d.h"

struct RenderInfo {
  bool isRenderingCollider = true;
};

enum class BodyUserData : unsigned int { Default = 0, Player, Enemy, Reward };
enum class FixtureUserData : unsigned int { Default = 0, FootSensor };

struct EntityPhysicsInfo {

  /*
  EntityPhysicsInfo(b2Vec2 dims, b2Vec2 startPos, b2BodyType type,
                    float density, float friction) {
    b2BodyUserData userData{};
    userData.pointer = (uintptr_t)&entityType;

    bodyDef.position = startPos;
    bodyDef.type = type;
    bodyDef.fixedRotation = true;

    polygonShape.SetAsBox(dims.x / 2.f, dims.y / 2.f);
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.shape = &polygonShape;
    boundingBoxDims = dims;
  }
  */

  EntityPhysicsInfo(
      b2Vec2 dims, std::pair<b2BodyDef, BodyUserData> &&body,
      std::vector<std::tuple<b2FixtureDef, b2PolygonShape, FixtureUserData>>
          &&fixtureShapeDefs)
      : bodyDefPair(std::move(body)),
        fixtureShapeDefs(std::move(fixtureShapeDefs)), boundingBoxDims(dims) {

    for (auto &tuple : this->fixtureShapeDefs) {
      // connect shape to fixture after initialization
      std::get<0>(tuple).shape = &std::get<1>(tuple);
      // connect fixture user data to fixture
      auto fixtureUserData = b2FixtureUserData();
      fixtureUserData.pointer = (uintptr_t)&std::get<2>(tuple);
      std::get<0>(tuple).userData = fixtureUserData;
    }

    auto bodyUserData = b2BodyUserData();
    bodyUserData.pointer = (uintptr_t)&bodyDefPair.second;
    bodyDefPair.first.userData = bodyUserData;
  }

  std::pair<b2BodyDef, BodyUserData> bodyDefPair;
  std::vector<std::tuple<b2FixtureDef, b2PolygonShape, FixtureUserData>>
      fixtureShapeDefs;
  const b2Vec2 boundingBoxDims;
  BodyUserData bodyUserData = BodyUserData::Default;
};

class Entity {
public:
  Entity(std::unique_ptr<EntityPhysicsInfo> physicsInfo);
  const std::shared_ptr<b2Body> body() const;
  const RenderInfo &renderInfo() const;
  const b2Vec2 &boxDims() const;
  virtual ~Entity() = default;

  const EntityPhysicsInfo &physicsInfo() const;

protected:
  std::unique_ptr<EntityPhysicsInfo> physicsInfo_;

  RenderInfo renderInfo_;
};

#endif // ENTITY_H

/*
 * Entity
 * PlayerEntity <- должен управляться как с клавиш, так и с эмуляции клавиш
 * EnemyEntity
 *
 */
