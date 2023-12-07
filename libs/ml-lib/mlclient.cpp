#include "mlclient.h"
#include <iostream>

MLClient::MLClient(QObject *parent) : QObject{parent} { std::cout << "hey!"; }
