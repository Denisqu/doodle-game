#include "gamescene.h"
#include "box2d/box2d.h"
#include "entity.h"
#include "entityconstructor.h"
#include "gamelogic.h"
#include "playerentity.h"
#include <QColor>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QRandomGenerator>
#include <algorithm>

GameScene::GameScene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent), mUpdateTimer{new QTimer(this)},
      logic_(GameLogic::GetInstance()) {

  connect(mUpdateTimer, &QTimer::timeout, this, &GameScene::update);
  connect(this, &GameScene::propagatePressedKey, logic_,
          &GameLogic::propagatePressedKey);

  // register callback
  logic_->addOnAddEntityCallback([this](const Entity &entity) {
    auto rect = QRectF(0, 0, entity.boxDims().x * GameScene::SceneScale,
                       entity.boxDims().y * GameScene::SceneScale);
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

  auto previousPlatformPosition =
      b2Vec2((double)sceneRect.center().x() / GameScene::SceneScale, 0.0f);
  for (int i = 0; i < 100; ++i) {
    auto xOffset = QRandomGenerator::global()->generateDouble() * 20 - 10;
    auto newPos = b2Vec2(previousPlatformPosition.x + xOffset,
                         previousPlatformPosition.y + 3);
    auto newPosQPoint = QPointF(newPos.x * GameScene::SceneScale,
                                newPos.y * GameScene::SceneScale);
    bool isInSceneBound = newPosQPoint.x() > sceneRect.bottomLeft().x() &&
                          newPosQPoint.x() < sceneRect.bottomRight().x();

    qDebug() << newPosQPoint;
    if (!isInSceneBound) {
      --i;
      continue;
    }

    auto platformBox = std::unique_ptr<Entity>(
        EntityConstructor::CreateStaticBox(b2Vec2(3.0f, 0.25f), newPos));
    previousPlatformPosition = newPos;

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
          body->GetPosition().x * GameScene::SceneScale,
          body->GetPosition().y * GameScene::SceneScale);
    }
  });

  // send player scene positions with signal
  auto rawPlayerPositions = logic_->getPlayerPositions();
  auto scenePlayerPositions = QVector<QPointF>();
  std::for_each(rawPlayerPositions.begin(), rawPlayerPositions.end(),
                [this, &scenePlayerPositions](b2Vec2 &el) {
                  scenePlayerPositions.push_back(
                      QPointF(el.x * GameScene::SceneScale,
                              el.y * GameScene::SceneScale));
                });
  emit playerPositionUpdated(scenePlayerPositions);

  isUpdating = false;
}
