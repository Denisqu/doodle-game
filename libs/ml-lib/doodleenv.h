#ifndef DOODLEENV_H
#define DOODLEENV_H

#include "ml-lib_global.h"
#include <QObject>
#include <memory>
#include <tuple>
#include <view.h>

enum class Actions : unsigned int { Left = 0, Right, Invalid };
Q_DECLARE_METATYPE(Actions)

// https://www.gymlibrary.dev/content/environment_creation/
class MLLIB_EXPORT DoodleEnv : public QObject {
  Q_OBJECT
public:
  DoodleEnv(QObject *parent = nullptr);
  using StepDataPtr = std::shared_ptr<std::tuple<QString, double, bool>>;

signals:
  void stepEnd(DoodleEnv::StepDataPtr stepData);
  void makeEnd();
  void resetEnd(QString state);

public slots:
  void make();
  void reset();
  void step(Actions action);

private:
  std::unique_ptr<doodlelib::View> view_;
  QImage screenImage;

  void sendFakeKeyPressEventToView(Actions action);
  QString getBase64ViewImage();
};

Q_DECLARE_METATYPE(DoodleEnv::StepDataPtr)

#endif // DOODLEENV_H
