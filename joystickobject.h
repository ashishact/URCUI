#ifndef JOYSTICKOBJECT_H
#define JOYSTICKOBJECT_H

#include <QObject>
#include <QTimer>
#include <QTimerEvent>
#include <gamepad/Gamepad.h>




class joystickObject: public QObject
{   Q_OBJECT
public:
    joystickObject();
    QTimer timer;
public slots:
    void timerEvent();
signals:
    void joystickFound(int n);
};


#endif // JOYSTICKOBJECT_H
