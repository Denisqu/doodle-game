#include "gamescene.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},
      mUpdateTimer{new QTimer(this)}
{




    connect(mUpdateTimer, &QTimer::timeout, this, &GameScene::update);
    mUpdateTimer->start(kUpdateTimeDeltaMs);
}

void GameScene::update()
{
    qDebug() << "update started";

    const auto testPixmap = new QPixmap("C:/Users/Denis/My Data/Projects/cxx-projects/qt-doodle-game/doodle-game/libs/doodle-lib/resources/img/characters.png");

    auto testItem = new QGraphicsPixmapItem(*testPixmap);



    this->addItem(testItem);

    qDebug() << "updated ended";
}
