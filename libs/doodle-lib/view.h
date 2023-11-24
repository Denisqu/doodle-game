#ifndef VIEW_H
#define VIEW_H
#include "doodle-lib_global.h"
#include <QGraphicsView>
#include <QObject>

namespace doodlelib {

class DOODLELIB_EXPORT View : public QGraphicsView {
  Q_OBJECT
public:
  explicit View(int w = 720, int h = 720, bool isManualUpdated = false,
                QWidget *widget = nullptr);

  void renderViewToImage(QImage &renderedImage);

  // костыль для обучения, нужно будет зарефакторить
  std::tuple<double, bool> getInfoForLearning();

public slots:
  void centerViewOnPlayer(QVector<QPointF> positions);

signals:
  void restartGame();
  void unpause();
  void pause();
  void pauseAfterUpdate();
  void manualSceneUpdate();

  // QWidget interface
protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  int h_;
  int w_;

private:
  QPointF oldCenter_;
  QRectF viewportRect_ = QRectF(QPointF(0, 0), QPointF(840, 840));
  bool isManualUpdated_ = false;
  bool isRestartedManually_ = false;
};

} // namespace doodlelib
#endif // VIEW_H
