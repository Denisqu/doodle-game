#ifndef VIEW_H
#define VIEW_H
#include <QObject>
#include <QGraphicsView>
#include "doodle-lib_global.h"

namespace doodlelib {


class DOODLELIB_EXPORT View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View();

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

}
#endif // VIEW_H
