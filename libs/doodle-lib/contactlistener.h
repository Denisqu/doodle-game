#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "box2d/box2d.h"

class ContactListener : public b2ContactListener {
public:
  ContactListener();

  // b2ContactListener interface
public:
  virtual void BeginContact(b2Contact *contact) override;
  virtual void EndContact(b2Contact *contact) override;

private:
  void handlePlayerBeginContact(b2Contact *contact);
  void handlePlayerEndContact(b2Contact *contact);
  void handleOneWayWallBeginContact(b2Contact *contact);
  void handleOneWayWallEndContact(b2Contact *contact);

  // b2ContactListener interface
  public:
  virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
  virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
};

#endif // CONTACTLISTENER_H
