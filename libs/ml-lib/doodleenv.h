#ifndef DOODLEENV_H
#define DOODLEENV_H

#include "ml-lib_global.h"
#include "view.h"
#include <QObject>
#include <memory>
#include <tuple>

enum class Actions : unsigned int { Left = 0, Right };

struct Screen {
  unsigned char data[120][120][3];
};

// https://pybind11.readthedocs.io/en/stable/classes.html
// https://www.gymlibrary.dev/content/environment_creation/
class MLLIB_EXPORT DoodleEnv : public QObject {
  Q_OBJECT
public:
  DoodleEnv(QObject *parent = nullptr);

public slots:
  void make();
  void reset();
  std::tuple<Screen *, double, bool> step(Actions action);

private:
  std::unique_ptr<doodlelib::View> view_;

  void sendFakeKeyPressEventToView(Actions action);
};
#endif // DOODLEENV_H
