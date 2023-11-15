#include "exampleservice.h"

ExampleService::ExampleService(QObject *parent)
    : QObject{parent}
{

}

int ExampleService::getOne()
{
    return 1;
}
