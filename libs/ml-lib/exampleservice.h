#ifndef EXAMPLESERVICE_H
#define EXAMPLESERVICE_H

#include <QObject>

class ExampleService : public QObject
{
    Q_OBJECT
public:
    explicit ExampleService(QObject *parent = nullptr);
    Q_INVOKABLE int getOne();
    // virtual ~ExampleService();

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

};

#endif // EXAMPLESERVICE_H
