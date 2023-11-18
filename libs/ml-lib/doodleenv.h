#ifndef DOODLEENV_H
#define DOODLEENV_H

#include "ml-lib_global.h"
#include "mlview.h"
#include <QObject>
#include <memory>
#include <tuple>

enum class Actions : unsigned int { Left = 0, Right };
Q_DECLARE_METATYPE(Actions)

struct Screen {
  unsigned char data[120][120][3];
};

// https://pybind11.readthedocs.io/en/stable/classes.html
// https://www.gymlibrary.dev/content/environment_creation/
class MLLIB_EXPORT DoodleEnv : public QObject {
  Q_OBJECT
public:
  DoodleEnv(QObject *parent = nullptr);

signals:
  void stepEnd(std::shared_ptr<std::tuple<Screen *, double, bool>> stepData);
  void makeEnd();
  void resetEnd();

public slots:
  void make();
  void reset();
  void step(Actions action);

private:
  std::unique_ptr<doodlelib::View> view_;

  void sendFakeKeyPressEventToView(Actions action);
};
#endif // DOODLEENV_H
