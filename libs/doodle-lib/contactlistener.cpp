#include "contactlistener.h"
#include "entity.h"
#include "gamelogic.h"
#include "playerentity.h"
#include <QDebug>
#include <QTimer>

ContactListener::ContactListener() {}

/*
 *
 *
 * class MyContactListener : public b2ContactListener
  {
      void BeginContact(b2Contact* contact) {
          //check if fixture A was the foot sensor
          void* fixtureUserData = contact->GetFixtureA()->GetUserData();
          if ( (int)fixtureUserData == 3 )
              numFootContacts++;
          //check if fixture B was the foot sensor
          fixtureUserData = contact->GetFixtureB()->GetUserData();
          if ( (int)fixtureUserData == 3 )
              numFootContacts++;
      }

void EndContact(b2Contact* contact) {
    //check if fixture A was the foot sensor
    void* fixtureUserData = contact->GetFixtureA()->GetUserData();
    if ( (int)fixtureUserData == 3 )
        numFootContacts--;
    //check if fixture B was the foot sensor
    fixtureUserData = contact->GetFixtureB()->GetUserData();
    if ( (int)fixtureUserData == 3 )
        numFootContacts--;
}
};
 *
 *
 *
 */

void ContactListener::BeginContact(b2Contact *contact) {
  handlePlayerBeginContact(contact);
}

void ContactListener::EndContact(b2Contact *contact) {
  handlePlayerEndContact(contact);
}

void ContactListener::handlePlayerBeginContact(b2Contact *contact) {
  auto bodyUserData =
      (BodyUserData *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
  auto fixtureUserData =
      (FixtureUserData *)contact->GetFixtureA()->GetUserData().pointer;
  auto body = contact->GetFixtureA()->GetBody();

  if (*bodyUserData == BodyUserData::Player &&
      *fixtureUserData == FixtureUserData::FootSensor && contact->IsEnabled()) {
    auto playerEntity = GameLogic::GetInstance()->playerEntityByBody_[body];
    playerEntity->increaseFootContacts();
    return;
  }

  bodyUserData =
      (BodyUserData *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
  fixtureUserData =
      (FixtureUserData *)contact->GetFixtureB()->GetUserData().pointer;
  body = contact->GetFixtureB()->GetBody();

  if (*bodyUserData == BodyUserData::Player &&
      *fixtureUserData == FixtureUserData::FootSensor && contact->IsEnabled()) {
    auto playerEntity = GameLogic::GetInstance()->playerEntityByBody_[body];
    playerEntity->increaseFootContacts();
    return;
  }
}

void ContactListener::handlePlayerEndContact(b2Contact *contact) {
  auto bodyUserData =
      (BodyUserData *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
  auto fixtureUserData =
      (FixtureUserData *)contact->GetFixtureA()->GetUserData().pointer;
  auto body = contact->GetFixtureA()->GetBody();

  if (*bodyUserData == BodyUserData::Player &&
      *fixtureUserData == FixtureUserData::FootSensor && contact->IsEnabled()) {
    auto playerEntity = GameLogic::GetInstance()->playerEntityByBody_[body];
    playerEntity->decreaseFootContacts();
    return;
  }

  bodyUserData =
      (BodyUserData *)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
  fixtureUserData =
      (FixtureUserData *)contact->GetFixtureB()->GetUserData().pointer;
  body = contact->GetFixtureB()->GetBody();

  if (*bodyUserData == BodyUserData::Player &&
      *fixtureUserData == FixtureUserData::FootSensor && contact->IsEnabled()) {
    auto playerEntity = GameLogic::GetInstance()->playerEntityByBody_[body];
    playerEntity->decreaseFootContacts();
    return;
  }
}

void ContactListener::handleOneWayWallBeginContact(b2Contact *contact) {
  b2Fixture *fixtureA = contact->GetFixtureA();
  b2Fixture *fixtureB = contact->GetFixtureB();

  auto userDataA = (BodyUserData *)fixtureA->GetBody()->GetUserData().pointer;
  auto userDataB = (BodyUserData *)fixtureB->GetBody()->GetUserData().pointer;

  // check if one of the fixtures is the platform
  b2Fixture *platformFixture = nullptr;
  b2Fixture *otherFixture = nullptr;
  if (*userDataA == BodyUserData::Platform) {
    platformFixture = fixtureA;
    otherFixture = fixtureB;
  } else if (*userDataB == BodyUserData::Platform) {
    platformFixture = fixtureB;
    otherFixture = fixtureA;
  }

  if (!platformFixture)
    return;

  // qDebug() << "platformFixture is not null";

  // b2Body *platformBody = platformFixture->GetBody();
  b2Body *otherBody = otherFixture->GetBody();
  int numPoints = contact->GetManifold()->pointCount;
  b2WorldManifold worldManifold;
  contact->GetWorldManifold(&worldManifold);

  // check if contact points are moving downward
  for (int i = 0; i < numPoints; i++) {
    b2Vec2 pointVel =
        otherBody->GetLinearVelocityFromWorldPoint(worldManifold.points[i]);
    // qDebug() << pointVel.x << " " << pointVel.y;
    if (pointVel.y < 0) {
      // qDebug() << "point is moving down, exit";
      return; // point is moving down, leave contact solid and exit
    }
  }

  // qDebug() << "disable platform collider";
  // no points are moving downward, contact should not be solid
  contact->SetEnabled(false);
}

void ContactListener::handleOneWayWallEndContact(b2Contact *contact) {
  // reset the default state of the contact in case it comes back for more
  // contact->SetEnabled(true);
  QTimer::singleShot(100, this, [contact]() {
    contact->SetEnabled(true);
    // qDebug() << "contact enabled!";
  });
}

void ContactListener::PreSolve(b2Contact *contact,
                               const b2Manifold *oldManifold) {
  handleOneWayWallBeginContact(contact);
}

void ContactListener::PostSolve(b2Contact *contact,
                                const b2ContactImpulse *impulse) {
  handleOneWayWallEndContact(contact);
}
