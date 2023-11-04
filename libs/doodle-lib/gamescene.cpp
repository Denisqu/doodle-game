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
#include <QRandomGenerator>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, mUpdateTimer{new QTimer(this)},
      logic_(GameLogic::GetInstance()) {

  connect(mUpdateTimer, &QTimer::timeout, this, &GameScene::update);
  logic_->addOnAddEntityCallback([this](const Entity &entity) {
    connect(this, &GameScene::propagatePressedKey, logic_,
            &GameLogic::propagatePressedKey);
    auto rect = QRectF(0, 0, entity.boxDims().x * this->sceneScale,
                       entity.boxDims().y * this->sceneScale);
    rect.moveCenter(QPoint(0, 0));
    entityToRectItemMap[&entity] = this->addRect(rect);
  });

  /*
  auto dynamicBox_0 = std::unique_ptr<Entity>(
      EntityConstructor::CreateDynamicBox(b2Vec2(1, 2), b2Vec2(50, 25)));
  auto dynamicBox_1 = std::unique_ptr<Entity>(
      EntityConstructor::CreateDynamicBox(b2Vec2(1, 3), b2Vec2(50, 75)));
  auto dynamicBox_2 = std::unique_ptr<Entity>(
      EntityConstructor::CreateDynamicBox(b2Vec2(2, 1), b2Vec2(50, 50)));
 */
  auto staticBox_0 = std::unique_ptr<Entity>(
      EntityConstructor::CreateStaticBox(b2Vec2(100.0f, 2.0f), b2Vec2(50, 0)));

  for (int i = 0; i < 10; ++i) {
    auto xOffset = QRandomGenerator::global()->generateDouble() * 8;
    auto staticBox = std::unique_ptr<Entity>(EntityConstructor::CreateStaticBox(
        b2Vec2(5.0f, 0.25f), b2Vec2(50 + xOffset * i, 3.5f * (1 + i))));
    logic_->addEntity(std::move(staticBox));
  }

  auto playerEntity = std::unique_ptr<Entity>(
      static_cast<Entity *>(EntityConstructor::CreatePlayerEntity(
          b2Vec2(0.5f, 0.5f), b2Vec2(50, 1), ControllerType::Human)));

  // logic_->addEntity(std::move(dynamicBox_0));
  // logic_->addEntity(std::move(dynamicBox_1));
  // logic_->addEntity(std::move(dynamicBox_2));
  logic_->addEntity(std::move(staticBox_0));
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
