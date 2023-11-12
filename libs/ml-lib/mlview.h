#ifndef MLVIEW_H
#define MLVIEW_H

#include "view.h"
#include <QTimer>

class MLView : public doodlelib::View {
  Q_OBJECT
public:
  explicit MLView();

private:
  QTimer renderToImageTimer;
};

#endif // MLVIEW_H
