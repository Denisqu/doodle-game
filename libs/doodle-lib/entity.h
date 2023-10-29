#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "box2d/box2d.h"

struct RenderInfo {
  bool isRenderingCollider = true;
};

struct EntityPhysicsInfo {

  EntityPhysicsInfo(b2Vec2 dims, b2Vec2 startPos, b2BodyType type,
                    float density, float friction) {
    bodyDef.position = startPos;
    bodyDef.type = type;
    polygonShape.SetAsBox(dims.x / 2.f, dims.y / 2.f);
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.shape = &polygonShape;
    boundingBoxDims = dims;
  }

  b2BodyDef bodyDef;
  b2PolygonShape polygonShape;
  b2FixtureDef fixtureDef;
  b2Vec2 boundingBoxDims;
};

class Entity {
public:
  Entity(std::unique_ptr<EntityPhysicsInfo> physicsInfo);
  const std::shared_ptr<b2Body> body() const;
  const RenderInfo &renderInfo() const;
  const b2Vec2 &boxDims() const;
  virtual ~Entity() = default;

  const EntityPhysicsInfo &physicsInfo() const;

private:
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
