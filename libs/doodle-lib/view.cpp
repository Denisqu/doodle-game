#include "view.h"
#include <QKeyEvent>
#include <QApplication>
#include "gamescene.h"

namespace doodlelib {

View::View()
    : QGraphicsView()
{
    auto scene = new GameScene(this);
    setScene(scene);
    resize(1280, 720);
    scale(1, -1);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/**
 * Exit application on Escape button
 */
void View::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat())
    {
        switch (event->key()) {
            case Qt::Key_Escape:
                QApplication::instance()->quit();
            break;
        }
    }
    QGraphicsView::keyPressEvent(event);
}

}
