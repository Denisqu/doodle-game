#include "gamescene.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "box2d/box2d.h"
#include "entity.h"
#include "gamelogic.h"

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

    entityToRectItemMap[entity] =
        this->addRect(QRectF(0, 0, entity.boxDims().x * this->sceneScale,
                             entity.boxDims().y * this->sceneScale));
  });

  auto bodyDef = b2BodyDef();
  bodyDef.position = b2Vec2(0, 0);
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

  mUpdateTimer->start(GameLogic::TimeStep);
}

QGraphicsRectItem *GameScene::getRectItemByEntity(const Entity &entity) {
  return entityToRectItemMap[entity];
}

void GameScene::update() {
  // update physics
  logic_->step();

  // render graphics on QGraphicsScene
  logic_->doOnActiveBody([this](b2Body *body) {
    auto entity = this->logic_->getEnityByBody(body);

    if (entity->renderInfo().isRenderingCollider) {
      // здесь нужно сопоставить entity с объектом, отвечающим за рендер, пока
      // что рендерится только один бокс на всех
      this->getRectItemByEntity(*entity)->setPos(body->GetPosition().x,
                                                 body->GetPosition().y);
    }
  });
}
