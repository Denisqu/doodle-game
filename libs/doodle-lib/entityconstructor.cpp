#include "entityconstructor.h"
#include "entity.h"

Entity *EntityConstructor::CreateStaticBox(b2Vec2 dims, b2Vec2 pos) {
  auto bodyDef = new b2BodyDef();
  bodyDef->position = b2Vec2(pos.x, pos.y);
  bodyDef->type = b2_staticBody;

  auto polygonShape = new b2PolygonShape();
  polygonShape->SetAsBox(dims.x / 2.f, dims.y / 2.f);

  auto fixtureDef = new b2FixtureDef();
  fixtureDef->density = 1.f;
  fixtureDef->friction = 0.7f;
  fixtureDef->shape = polygonShape;

  auto entity = new Entity(std::unique_ptr<b2BodyDef>(bodyDef),
                           std::unique_ptr<b2FixtureDef>(fixtureDef), dims);

  return entity;
}

Entity *EntityConstructor::CreateDynamicBox(b2Vec2 dims, b2Vec2 pos) {
  auto bodyDef = new b2BodyDef();
  bodyDef->position = b2Vec2(pos.x, pos.y);
  bodyDef->type = b2_dynamicBody;

  auto polygonShape = new b2PolygonShape();
  polygonShape->SetAsBox(dims.x / 2.f, dims.y / 2.f);

  auto fixtureDef = new b2FixtureDef();
  fixtureDef->density = 1.f;
  fixtureDef->friction = 0.7f;
  fixtureDef->shape = polygonShape;

  auto entity = new Entity(std::unique_ptr<b2BodyDef>(bodyDef),
                           std::unique_ptr<b2FixtureDef>(fixtureDef), dims);

  return entity;
}

EntityConstructor::EntityConstructor() {}
