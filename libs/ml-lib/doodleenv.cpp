#include "doodleenv.h"

DoodleEnv::DoodleEnv() {}

void DoodleEnv::make() { view_ = std::make_unique<doodlelib::View>(); }

void DoodleEnv::reset() {
  view_.reset();
  this->make();
}

/*
 * python::env.step(action) -> mllib::doodleenv.step(action):
 * 1) Совершаем действие
 * 2) Вызываем руками GameScene::update()
 * 3) Рендерим view в картинку
 * (см. https://doc.qt.io/qt-5/qgraphicsview.html#render)
 * 4) Возвращаем nextState, reward, done
 */
std::tuple<Screen *, double, bool> DoodleEnv::step(Actions action) {}
