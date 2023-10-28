#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "box2d/box2d.h"

struct RenderInfo {
  bool isRenderingCollider = true;
};

class Entity {
public:
  Entity(std::unique_ptr<b2BodyDef> bodyDef,
         std::unique_ptr<b2FixtureDef> fixtureDef, b2Vec2 boxDims);

  const std::shared_ptr<b2Body> body() const;
  const RenderInfo &renderInfo() const;
  const b2PolygonShape &pPolygonShape() const;
  const b2Vec2 &boxDims() const;
  const b2BodyDef &bodyDef() const;
  const b2FixtureDef &fixtureDef() const;

  virtual ~Entity() = default;

private:
  const std::unique_ptr<b2BodyDef> pBodyDef_;
  const std::unique_ptr<b2FixtureDef> pFixtureDef_;
  const std::unique_ptr<b2PolygonShape> pPolygonShape_;
  // const std::shared_ptr<b2Body> pBody_;
  const b2Vec2 boxDims_;

  RenderInfo renderInfo_;
};

#endif // ENTITY_H

/*
 * Entity
 * PlayerEntity <- должен управляться как с клавиш, так и с эмуляции клавиш
 * EnemyEntity
 *
 */
