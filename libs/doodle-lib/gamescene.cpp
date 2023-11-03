#include "gamescene.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "box2d/box2d.h"
#include "entity.h"
#include "entityconstructor.h"
#include "gamelogic.h"
#include "playerentity.h"
#include <QDebug>
#include <QKeyEvent>
/** We need this to easily convert between pixel and real-world coordinates*/
// static const float SCALE = 30.f;

/*
void CreateGround(b2World &World, float X, float Y) {
  b2BodyDef BodyDef;
  BodyDef.position = b2Vec2(X, Y);
  BodyDef.type = b2_staticBody;
  b2Body *Body = World.CreateBody(&BodyDef);
  b2PolygonShape Shape;

  Shape.SetAsBox(
      10.f / 2,
      1.f /
          2); // Creates a box shape. Divide your desired width and height by 2.
  b2FixtureDef FixtureDef;
  FixtureDef.density = 0.f;         // Sets the density of the body
  FixtureDef.shape = &Shape;        // Sets the shape
  Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void CreateBox(b2World &World, int x, int y) {
  b2BodyDef BodyDef;
  BodyDef.position = b2Vec2(x, y);
  BodyDef.type = b2_dynamicBody;
  b2Body *Body = World.CreateBody(&BodyDef);

  b2PolygonShape Shape;
  Shape.SetAsBox(1.f / 2, 1.f / 2);
  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 0.7f;
  FixtureDef.shape = &Shape;
  Body->CreateFixture(&FixtureDef);
}

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, mUpdateTimer{new QTimer(this)},
      world(b2Vec2(0.0, -10.0f)) {
  CreateGround(world, 0, 0);
  CreateBox(world, 0, 10);
  // 0,0 - topleft corner of rectangle
  rectBox = this->addRect(QRectF(0, 0, 1 * SCALE, 1 * SCALE));
  rectGround = this->addRect(QRectF(0, 0, 10 * SCALE, 1 * SCALE));

  connect(mUpdateTimer, &QTimer::timeout, this, &GameScene::update);
  mUpdateTimer->start(kUpdateTimeDeltaMs);
}
*/
/*
void GameScene::update() {
  // qDebug() << "update started";

  world.Step(1 / 60.f, 8, 3);

  // const auto testPixmap = new QPixmap("C:/Users/Denis/My
  //
Data/Projects/cxx-projects/qt-doodle-game/doodle-game/libs/doodle-lib/resources/img/characters.png");
  // auto testItem = new QGraphicsPixmapItem(*testPixmap);
  // this->addItem(testItem);

  for (b2Body *BodyIterator = world.GetBodyList(); BodyIterator != 0;
       BodyIterator = BodyIterator->GetNext()) {
    if (BodyIterator->GetType() == b2_dynamicBody) {

      // sf::Sprite Sprite;
      // Sprite.SetTexture(BoxTexture);
      // Sprite.SetOrigin(16.f, 16.f);
      // Sprite.SetPosition(SCALE * BodyIterator->GetPosition().x, SCALE *
      // BodyIterator->GetPosition().y);
      // Sprite.SetRotation(BodyIterator->GetAngle() * 180/b2_pi);
      // Window.Draw(Sprite);
      //++BodyCount;
      rectBox->setPos(QPointF(BodyIterator->GetPosition().x * SCALE,
                              BodyIterator->GetPosition().y * SCALE));

    } else {
      // sf::Sprite GroundSprite;
      // GroundSprite.SetTexture(GroundTexture);
      // GroundSprite.SetOrigin(400.f, 8.f);
      // GroundSprite.SetPosition(BodyIterator->GetPosition().x * SCALE,
      // BodyIterator->GetPosition().y * SCALE);
      // GroundSprite.SetRotation(180/b2_pi * BodyIterator->GetAngle());
      // Window.Draw(GroundSprite);

      rectGround->setPos(QPointF(BodyIterator->GetPosition().x * SCALE,
                                 BodyIterator->GetPosition().y * SCALE));
    }
  }

  // qDebug() << "update ended";
}
*/

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, mUpdateTimer{new QTimer(this)},
      logic_(GameLogic::GetInstance()) {

  connect(mUpdateTimer, &QTimer::timeout, this, &GameScene::update);
  logic_->addOnAddEntityCallback([this](const Entity &entity) {
    // rectBox = this->addRect(QRectF(0, 0, 1 * SCALE, 1 * SCALE));

    // нужно не только создавать rect, но и добавлять в таблицу [Entity,
    // QGraphicsRectItem *]

    connect(this, &GameScene::propagatePressedKey, logic_,
            &GameLogic::propagatePressedKey);
    auto rect = QRectF(0, 0, entity.boxDims().x * this->sceneScale,
                       entity.boxDims().y * this->sceneScale);
    rect.moveCenter(QPoint(0, 0));
    entityToRectItemMap[&entity] = this->addRect(rect);
  });

  /*
  auto bodyDef = b2BodyDef();
  bodyDef.position = b2Vec2(0, 10);
  bodyDef.type = b2_dynamicBody;

  auto polygonShape = b2PolygonShape();
  polygonShape.SetAsBox(1, 1);

  auto fixtureDef = b2FixtureDef();
  fixtureDef.density = 1.f;
  fixtureDef.friction = 0.7f;
  fixtureDef.shape = &polygonShape;

  // b2Body *body = World.CreateBody(&bodyDef);
  // body->CreateFixture(&fixtureDef);

  auto boxEntity = std::make_unique<Entity>(
      std::make_unique<b2BodyDef>(bodyDef),
      std::make_unique<b2FixtureDef>(fixtureDef), b2Vec2(1, 1));

  // auto groundEntity;
  logic_->addEntity(std::move(boxEntity));

  bodyDef.position = b2Vec2(0, 0);
  bodyDef.type = b2_staticBody;
  auto boxEntity2 = std::make_unique<Entity>(
      std::make_unique<b2BodyDef>(bodyDef),
      std::make_unique<b2FixtureDef>(fixtureDef), b2Vec2(1, 1));
  logic_->addEntity(std::move(boxEntity2));
  */
  auto dynamicBox_0 = std::unique_ptr<Entity>(
      EntityConstructor::CreateDynamicBox(b2Vec2(1, 2), b2Vec2(50, 25)));
  auto dynamicBox_1 = std::unique_ptr<Entity>(
      EntityConstructor::CreateDynamicBox(b2Vec2(1, 3), b2Vec2(50, 75)));
  auto dynamicBox_2 = std::unique_ptr<Entity>(
      EntityConstructor::CreateDynamicBox(b2Vec2(2, 1), b2Vec2(50, 50)));
  auto staticBox_0 = std::unique_ptr<Entity>(
      EntityConstructor::CreateStaticBox(b2Vec2(100.0f, 2.0f), b2Vec2(50, 0)));
  auto staticBox_1 = std::unique_ptr<Entity>(
      EntityConstructor::CreateStaticBox(b2Vec2(2.0f, 0.5f), b2Vec2(25, 4)));

  auto playerEntity = std::unique_ptr<Entity>(
      static_cast<Entity *>(EntityConstructor::CreatePlayerEntity(
          b2Vec2(0.5f, 2), b2Vec2(50, 1), ControllerType::Human)));

  logic_->addEntity(std::move(dynamicBox_0));
  logic_->addEntity(std::move(dynamicBox_1));
  logic_->addEntity(std::move(dynamicBox_2));
  logic_->addEntity(std::move(staticBox_0));
  logic_->addEntity(std::move(staticBox_1));
  logic_->addEntity(std::move(playerEntity));
  mUpdateTimer->start(GameLogic::TimeStep * 1000);
}

QGraphicsRectItem *GameScene::getRectItemByEntity(const Entity &entity) {
  return entityToRectItemMap[&entity];
}

void GameScene::keyPressEvent(QKeyEvent *event) {
  emit propagatePressedKey(event->key());
}

void GameScene::update() {
  // update logic
  logic_->step();

  // render graphics on QGraphicsScene
  logic_->doOnActiveBody([this](b2Body *body) {
    auto entity = this->logic_->getEnityByBody(body);

    if (entity->renderInfo().isRenderingCollider) {

      /*
    qDebug() << "rendering this entity: " << entity.get()
             << "with this rect: " << this->getRectItemByEntity(*entity)
             << "its real pos:" << body->GetPosition().x << " "
             << body->GetPosition().y;
      */

      this->getRectItemByEntity(*entity)->setPos(
          (body->GetPosition().x - 0 * entity->boxDims().x / 2) * sceneScale,
          (body->GetPosition().y - 0 * entity->boxDims().y / 2) * sceneScale);
    }
  });
}
