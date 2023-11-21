#include "doodleenv.h"
#include "gamelogic.h"
#include "gamescene.h"
#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QKeyEvent>

DoodleEnv::DoodleEnv(QObject *parent) : QObject(parent) {
  qRegisterMetaType<Actions>();
  qRegisterMetaType<StepDataPtr>();
}

/*
 * Создаём
 *
 */
void DoodleEnv::make() {
  qDebug() << "make in doodle env!!!";
  if (view_.get())
    return;
  view_ = std::make_unique<doodlelib::View>(84, 84);
  emit view_->pause();

  view_->show();
  emit makeEnd();
}

void DoodleEnv::reset() {
  if (!view_.get())
    return;
  emit view_.get()->restartGame();

  QString state = getBase64ViewImage();
  emit resetEnd(state);
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
  // emit view_->pauseAfterUpdate();
  sendFakeKeyPressEventToView(action);
  emit view_->unpause();
  emit view_->manualSceneUpdate();
  emit view_->pause();

  QString base64Image = getBase64ViewImage();

  emit stepEnd(std::make_shared<std::tuple<QString, double, bool>>(base64Image,
                                                                   100, false));
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

  QApplication::sendEvent(view_.get(), keyEvent);
}

QString DoodleEnv::getBase64ViewImage() {
  screenImage = QImage(view_->size(), QImage::Format::Format_RGB16);
  view_->renderViewToImage(screenImage);
  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  screenImage.save(&buffer, "PNG");
  return QString::fromLatin1(byteArray.toBase64().data());
}
