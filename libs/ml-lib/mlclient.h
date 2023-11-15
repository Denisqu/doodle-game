#ifndef MLCLIENT_H
#define MLCLIENT_H

#include <QObject>

class MLClient : public QObject
{
    Q_OBJECT
public:
    explicit MLClient(QObject *parent = nullptr);

signals:

};

#endif // MLCLIENT_H
