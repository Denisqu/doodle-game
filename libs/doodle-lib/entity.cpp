#include "entity.h"
#include "box2d/box2d.h"

Entity::Entity(std::unique_ptr<b2BodyDef> bodyDef,
               std::unique_ptr<b2FixtureDef> fixtureDef, b2Vec2 boxDims)
    : pBodyDef_(std::move(bodyDef)), pFixtureDef_(std::move(fixtureDef)),
      pPolygonShape_(new b2PolygonShape()), boxDims_(boxDims) {}

/*
Entity::Entity(const b2BodyDef &bodyDef, const b2FixtureDef &fixtureDef,
               const b2Vec2 &boxDims)
    : pBodyDef_(std::make_unique<b2BodyDef>(bodyDef)),
      pFixtureDef_(std::make_unique()), pPolygonShape_()

{}
*/

const RenderInfo &Entity::renderInfo() const { return renderInfo_; }

const b2PolygonShape &Entity::pPolygonShape() const { return *pPolygonShape_; }

const b2Vec2 &Entity::boxDims() const { return boxDims_; }

const b2BodyDef &Entity::bodyDef() const { return *pBodyDef_; }

const b2FixtureDef &Entity::fixtureDef() const { return *pFixtureDef_; }
