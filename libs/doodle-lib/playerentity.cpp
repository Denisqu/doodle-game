#include "playerentity.h"

PlayerEntity::PlayerEntity(std::unique_ptr<b2BodyDef> bodyDef,
                           std::unique_ptr<b2FixtureDef> fixtureDef,
                           b2Vec2 boxDims)
    : Entity(std::move(bodyDef), std::move(fixtureDef), std::move(boxDims)) {}
