#ifndef VIEW_H
#define VIEW_H
#include "doodle-lib_global.h"
#include <QGraphicsView>
#include <QObject>

namespace doodlelib {

class DOODLELIB_EXPORT View : public QGraphicsView {
  Q_OBJECT
public:
  explicit View(int w = 720, int h = 720, QWidget *widget = nullptr);

public slots:
  void centerViewOnPlayer(QVector<QPointF> positions);

signals:
  void restartGame();
  void unpause();
  void pause();
  void pauseAfterUpdate();

  // QWidget interface
protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  int h_;
  int w_;

private:
  QPointF oldCenter;
};

} // namespace doodlelib
#endif // VIEW_H
