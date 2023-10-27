#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>

#include "box2d/box2d.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

private:
    const double kUpdateTimeDeltaMs = 16.67;
    QTimer* mUpdateTimer;
    b2World world;
    QGraphicsRectItem* rectBox;
    QGraphicsRectItem* rectGround;

private slots:
    void update();

};

#endif // GAMESCENE_H
