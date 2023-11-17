#include "doodleapplication.h"
#include <QThread>

DoodleApplication::~DoodleApplication() {
  if (serverThread) {
    serverThread->quit();
    serverThread->wait(1000);
    serverThread->terminate();
  }
}
