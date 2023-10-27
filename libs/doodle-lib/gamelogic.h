#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include "box2d/box2d.h"

class Entity;

class GameLogic
{
public:
    explicit GameLogic();

private:

    // physics:
    b2Vec2 gravity = b2Vec2(0.0, -10.0f);
    b2World world = b2World(gravity);
};

#endif // GAMELOGIC_H
