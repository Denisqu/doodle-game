#include "entity.h"
#include "box2d/box2d.h"


Entity::Entity(const b2BodyDef &bodyDef, const b2FixtureDef &fixtureDef, const b2PolygonShape &polygonShape):
    bodyDef_(bodyDef),
    fixtureDef_(fixtureDef),
    polygonShape_(polygonShape)
{

}
