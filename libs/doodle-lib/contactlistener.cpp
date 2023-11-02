#include "contactlistener.h"
#include "entity.h"
#include "gamelogic.h"
#include "playerentity.h"

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

  auto bodyUserData =
      (BodyUserData *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
  auto fixtureUserData =
      (FixtureUserData *)contact->GetFixtureA()->GetUserData().pointer;
  auto body = contact->GetFixtureA()->GetBody();

  if (*bodyUserData == BodyUserData::Player &&
      *fixtureUserData == FixtureUserData::FootSensor) {
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
      *fixtureUserData == FixtureUserData::FootSensor) {
    auto playerEntity = GameLogic::GetInstance()->playerEntityByBody_[body];
    playerEntity->increaseFootContacts();
    return;
  }
}

void ContactListener::EndContact(b2Contact *contact) {
  auto bodyUserData =
      (BodyUserData *)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
  auto fixtureUserData =
      (FixtureUserData *)contact->GetFixtureA()->GetUserData().pointer;
  auto body = contact->GetFixtureA()->GetBody();

  if (*bodyUserData == BodyUserData::Player &&
      *fixtureUserData == FixtureUserData::FootSensor) {
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
      *fixtureUserData == FixtureUserData::FootSensor) {
    auto playerEntity = GameLogic::GetInstance()->playerEntityByBody_[body];
    playerEntity->decreaseFootContacts();
    return;
  }
}
