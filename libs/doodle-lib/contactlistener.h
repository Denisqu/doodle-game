#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "box2d/box2d.h"

class ContactListener : public b2ContactListener
{
public:
    ContactListener();

    // b2ContactListener interface
public:
    virtual void BeginContact(b2Contact *contact) override;
    virtual void EndContact(b2Contact *contact) override;
};

#endif // CONTACTLISTENER_H
