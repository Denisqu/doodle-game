#include "entity.h"

/*
Entity::Entity(b2BodyDef bodyDef, b2FixtureDef fixtureDef, b2Vec2 boxDims)
    : bodyDef_(bodyDef), fixtureDef_(fixtureDef), boxDims_(boxDims) {}
*/

/*
Entity::Entity(const b2BodyDef &bodyDef, const b2FixtureDef
&fixtureDef, const b2Vec2 &boxDims) :
pBodyDef_(std::make_unique<b2BodyDef>(bodyDef)),
      pFixtureDef_(std::make_unique()), pPolygonShape_()

{}
*/

Entity::Entity(std::unique_ptr<EntityPhysicsInfo> physicsInfo)
    : physicsInfo_(std::move(physicsInfo)) {}

const RenderInfo &Entity::renderInfo() const { return renderInfo_; }

const b2Vec2 &Entity::boxDims() const { return physicsInfo_->boundingBoxDims; }

const EntityPhysicsInfo &Entity::physicsInfo() const { return *physicsInfo_; }
