#ifndef DOODLEENV_H
#define DOODLEENV_H

#include "gamescene.h"
#include "view.h"
#include <memory>
#include <tuple>

enum class Actions : unsigned int { Left = 0, Right };

struct Screen {
  unsigned char data[120][120][3];
};

// https://pybind11.readthedocs.io/en/stable/classes.html
// https://www.gymlibrary.dev/content/environment_creation/
class DoodleEnv {
public:
  DoodleEnv();
  void make();
  void reset();

  std::tuple<Screen *, double, bool> step(Actions action);

private:
  // Будет жить в другом треде
  std::unique_ptr<doodlelib::View> view_;
};
#endif // DOODLEENV_H
