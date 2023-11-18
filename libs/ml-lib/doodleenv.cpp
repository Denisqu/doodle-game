#include "doodleenv.h"
#include "gamescene.h"
#include <QApplication>
#include <QKeyEvent>

DoodleEnv::DoodleEnv(QObject *parent) : QObject(parent) {}

/*
 * Создаём
 *
 */
void DoodleEnv::make() {
  view_ = std::make_unique<doodlelib::View>();
  view_->show();
  emit makeEnd();
}

void DoodleEnv::reset() {
  view_.reset();
  this->make();
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

  emit stepEnd(std::tuple<Screen *, double, bool>());
}

void DoodleEnv::sendFakeKeyPressEventToView(Actions action) {
  // QKeyEvent *key_press =
  //    new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "X");
  //                                                                          text
  //                                                                          ─────┘
  // QApplication::sendEvent(ui->lineEdit, key_press);
}
