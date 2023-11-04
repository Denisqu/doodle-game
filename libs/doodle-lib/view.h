#ifndef VIEW_H
#define VIEW_H
#include "doodle-lib_global.h"
#include <QGraphicsView>
#include <QObject>

namespace doodlelib {

class DOODLELIB_EXPORT View : public QGraphicsView {
  Q_OBJECT
public:
  explicit View();

public slots:
  void centerViewOnPlayer(QVector<QPointF> positions);

  // QWidget interface
protected:
  virtual void keyPressEvent(QKeyEvent *event) override;

private:
  QPointF oldCenter;
};

} // namespace doodlelib
#endif // VIEW_H
