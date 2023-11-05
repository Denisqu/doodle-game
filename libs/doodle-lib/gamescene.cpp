#include "gamescene.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "box2d/box2d.h"
#include "entity.h"
#include "entityconstructor.h"
#include "gamelogic.h"
#include "playerentity.h"
#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <algorithm>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, mUpdateTimer{new QTimer(this)},
      logic_(GameLogic::GetInstance()) {

  connect(mUpdateTimer, &QTimer::timeout, this, &GameScene::update);
  connect(this, &GameScene::propagatePressedKey, logic_,
          &GameLogic::propagatePressedKey);

  // register callback
  logic_->addOnAddEntityCallback([this](const Entity &entity) {
    auto rect = QRectF(0, 0, entity.boxDims().x * this->sceneScale,
                       entity.boxDims().y * this->sceneScale);
    rect.moveCenter(QPoint(0, 0));
    auto penColor = entity.physicsInfo().bodyUserData == BodyUserData::Player
                        ? QColor(0, 255, 0)
                        : QColor(0, 0, 0);
    auto brushColor = penColor;
    brushColor.setAlpha(50);
    auto pen = QPen(penColor);
    auto brush = QBrush(brushColor, Qt::BrushStyle::SolidPattern);
    entityToRectItemMap[&entity] = this->addRect(rect, pen, brush);
  });

  auto groundBox = std::unique_ptr<Entity>(
      EntityConstructor::CreateStaticBox(b2Vec2(100.0f, 2.0f), b2Vec2(50, 0)));

  for (int i = 0; i < 10; ++i) {
    auto xOffset = QRandomGenerator::global()->generateDouble() * 8;
    auto platformBox =
        std::unique_ptr<Entity>(EntityConstructor::CreateStaticBox(
            b2Vec2(5.0f, 0.25f), b2Vec2(50 + xOffset * i, 3.0f * (1 + i))));
    logic_->addEntity(std::move(platformBox));
  }

  auto playerEntity = std::unique_ptr<Entity>(
      static_cast<Entity *>(EntityConstructor::CreatePlayerEntity(
          b2Vec2(0.5f, 0.5f), b2Vec2(50, 1), ControllerType::Human)));

  logic_->addEntity(std::move(groundBox));
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
  if (isUpdating)
    return;
  isUpdating = true;

  // update logic
  logic_->step();

  // render graphics on QGraphicsScene
  logic_->doOnActiveBody([this](b2Body *body) {
    auto entity = this->logic_->getEnityByBody(body);
    if (entity->renderInfo().isRenderingCollider) {
      this->getRectItemByEntity(*entity)->setPos(
          body->GetPosition().x * sceneScale,
          body->GetPosition().y * sceneScale);
    }
  });

  // send player scene positions with signal
  auto rawPlayerPositions = logic_->getPlayerPositions();
  auto scenePlayerPositions = QVector<QPointF>();
  std::for_each(rawPlayerPositions.begin(), rawPlayerPositions.end(),
                [this, &scenePlayerPositions](b2Vec2 &el) {
                  scenePlayerPositions.push_back(
                      QPointF(el.x * sceneScale, el.y * sceneScale));
                });
  emit playerPositionUpdated(scenePlayerPositions);

  isUpdating = false;
}
