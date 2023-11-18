#include "doodleenv.h"
#include "gamelogic.h"
#include "gamescene.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

DoodleEnv::DoodleEnv(QObject *parent) : QObject(parent) {
  qRegisterMetaType<Actions>();
}

/*
 * Создаём
 *
 */
void DoodleEnv::make() {
  qDebug() << "make in doodle env!!!";
  if (view_.get())
    return;
  view_ = std::make_unique<doodlelib::View>();

  view_->show();
  emit makeEnd();
}

void DoodleEnv::reset() {
  if (!view_.get())
    return;
  emit view_.get()->restartGame();
  emit resetEnd();
}

/*
 * python::env.step(action) -> mllib::doodleenv.step(action):
 * 1) GameScene::pauseAfterUpdate() +
 * 2) emit fakeKeyPressEvent(Actions action)
 * 3) unpause()
 * 4) Рендерим view в картинку
 * (см. https://doc.qt.io/qt-5/qgraphicsview.html#render)
 * 5) Возвращаем nextState, reward, done
 */
void DoodleEnv::step(Actions action) {
  sendFakeKeyPressEventToView(action);
  emit stepEnd(std::make_shared<std::tuple<Screen *, double, bool>>());
}

void DoodleEnv::sendFakeKeyPressEventToView(Actions action) {
  QKeyEvent *keyEvent = nullptr;
  switch (action) {
  case Actions::Left:
    keyEvent =
        new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
    break;
  case Actions::Right:
    keyEvent =
        new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_D, Qt::NoModifier, "D");
    break;
  default:
    break;
  }

  qDebug() << "sending fakeKeyPressEvent! " << keyEvent->key();
  QApplication::sendEvent(view_.get(), keyEvent);
}
