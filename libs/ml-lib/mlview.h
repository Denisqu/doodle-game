#ifndef MLVIEW_H
#define MLVIEW_H

#include "ml-lib_global.h"
#include "view.h"
#include <QTimer>

enum class Actions : unsigned int;

class MLLIB_EXPORT MLView : public doodlelib::View {
  Q_OBJECT
  using doodlelib::View::View;

public:
  // explicit MLView(QWidget *parent = nullptr);

private:
  QTimer renderToImageTimer;
};

#endif // MLVIEW_H
