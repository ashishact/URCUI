#ifndef JOYDATA_H
#define JOYDATA_H


#include <QObject>
#define MaxNoButton 8//but 12 button are available
#define MaxNoAxis 6


class JoyData: public QObject
{Q_OBJECT
public:
    JoyData();
    bool button[2][MaxNoButton];    // maximum 2 joystick
    float axis[2][MaxNoAxis];
//    unsigned int changedAxisNo;
//    unsigned int device;
    bool deviceAttachedorRemoved;

//    bool btnEvent;
signals:
    void changed(unsigned int device, bool btnEvent,unsigned int axisNo);
};

#endif // JOYDATA_H
