#include "debugobject.h"

DebugObject::DebugObject(QObject *parent): QObject{parent}{

}

void DebugObject::debugSlot(){
    qDebug() << "Debug message #" << i;
    i++;
}
