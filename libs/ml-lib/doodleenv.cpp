#include "doodleenv.h"
#include <QApplication>
#include <QKeyEvent>

DoodleEnv::DoodleEnv() {}

/*
 * Создаём
 *
 */
void DoodleEnv::make() { view_ = std::make_unique<doodlelib::View>(); }

void DoodleEnv::reset() {
  view_.reset();
  this->make();
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
std::tuple<Screen *, double, bool> DoodleEnv::step(Actions action) {
  return std::tuple<Screen *, double, bool>();
}

void DoodleEnv::sendFakeKeyPressEventToView(Actions action) {
  // QKeyEvent *key_press =
  //    new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "X");
  //                                                                          text
  //                                                                          ─────┘
  // QApplication::sendEvent(ui->lineEdit, key_press);
}
