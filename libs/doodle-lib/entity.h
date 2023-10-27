#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

#include "box2d/box2d.h"


class Entity
{
public:
    explicit Entity(const b2BodyDef& bodyDef,
                    const b2FixtureDef& fixtureDef,
                    const b2PolygonShape& polygonShape);

private:
    const b2BodyDef bodyDef_;
    const b2FixtureDef fixtureDef_;
    const b2PolygonShape polygonShape_;



};

#endif // ENTITY_H
