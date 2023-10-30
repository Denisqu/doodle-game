#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <vector>

#include "box2d/box2d.h"

struct RenderInfo {
  bool isRenderingCollider = true;
};

enum class BodyUserData : unsigned int { Platform = 0, Player, Enemy, Reward };

enum class FixtureType : unsigned int { Default = 0, FootSensor };

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
      b2Vec2 dims, BodyUserData buData, std::unique_ptr<b2BodyDef> &&body,
      std::vector<std::pair<b2FixtureDef, b2PolygonShape>> &&fixtureShapeDefs)
      : bodyDef(std::move(body)), fixtureShapeDefs(std::move(fixtureShapeDefs)),
        boundingBoxDims(dims) {
    // connect shape to fixture after initialization
    for (auto &pair : this->fixtureShapeDefs) {
      pair.first.shape = &pair.second;
    }
  }

  std::unique_ptr<b2BodyDef> bodyDef;
  std::vector<std::pair<b2FixtureDef, b2PolygonShape>> fixtureShapeDefs;
  const b2Vec2 boundingBoxDims;
  BodyUserData entityType = BodyUserData::Platform;
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
