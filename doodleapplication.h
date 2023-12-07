#ifndef DOODLEAPPLICATION_H
#define DOODLEAPPLICATION_H

#include <QObject>
#include <QApplication>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class DoodleApplication : public QApplication
{
    Q_OBJECT
public:
    using QApplication::QApplication;
    ~DoodleApplication();

public:
    QThread* serverThread = nullptr;

};

#endif // DOODLEAPPLICATION_H
