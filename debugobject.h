#ifndef DEBUGOBJECT_H
#define DEBUGOBJECT_H

#include <QObject>
#include <QDebug>
#include <QObject>

class DebugObject : public QObject
{
    Q_OBJECT
    int32_t i = 0;

public:
    explicit DebugObject(QObject *parent = nullptr);

public slots:
    void debugSlot();
};

#endif // DEBUGOBJECT_H
