#ifndef ENTITYCONSTRUCTOR_H
#define ENTITYCONSTRUCTOR_H

#include "box2d/box2d.h"
class Entity;

class EntityConstructor
{
public:
    static Entity* CreateStaticBox(b2Vec2 dims, b2Vec2 pos);
    static Entity* CreateDynamicBox(b2Vec2 dims, b2Vec2 pos);

private:
    EntityConstructor();
};

#endif // ENTITYCONSTRUCTOR_H
