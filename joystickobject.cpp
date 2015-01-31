#include "joystickobject.h"

#include <QTimer>
#include "gamepad/Gamepad.h"
#include "joydata.h"

JoyData joy;

void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context);
void onDeviceAttached(struct Gamepad_device * device, void * context);
void onDeviceRemoved(struct Gamepad_device * device, void * context);



void onButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) {

    unsigned int deviceID = device->deviceID;    // local deviceID
    if( deviceID< 2){   // max two joystick
        joy.button[deviceID][buttonID]=true;
        emit joy.changed(deviceID,true,0);
    }
    else {
       // qDebug()<<" Device ID  > 1";
    }
}

void onButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) {

    unsigned int deviceID = device->deviceID;    // local deviceID
    if( deviceID< 2){   // max two joystick
        joy.button[deviceID][buttonID]=false;
        emit joy.changed(deviceID,true,0);
    }
    else {
        //qDebug()<<" Device ID  > 1";
    }

}

 void onAxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context) {

        //printf("Axis %u moved from %f to %f on device %u at %f with context %p\n", axisID, lastValue, value, device->deviceID, timestamp, context);

     unsigned int deviceID = device->deviceID;    // local deviceID
     if( deviceID< 2){   // max two joystick
         joy.axis[deviceID][axisID]=value;
         emit joy.changed(deviceID,false,axisID);
     }
     else {
         //qDebug()<<" Device ID  > 1";
     }

}

 void onDeviceAttached(struct Gamepad_device * device, void * context) {

        //printf("Device ID %u attached (vendor = 0x%X; product = 0x%X) with context %p\n", device->deviceID, device->vendorID, device->productID, context);
    joy.deviceAttachedorRemoved=true;
}

 void onDeviceRemoved(struct Gamepad_device * device, void * context) {

        //printf("Device ID %u removed with context %p\n", device->deviceID, context);
    joy.deviceAttachedorRemoved=true;
}

void initGamepad() {
    Gamepad_deviceAttachFunc(onDeviceAttached, (void *) 0x1);
    Gamepad_deviceRemoveFunc(onDeviceRemoved, (void *) 0x2);
    Gamepad_buttonDownFunc(onButtonDown, (void *) 0x3);
    Gamepad_buttonUpFunc(onButtonUp, (void *) 0x4);
    Gamepad_axisMoveFunc(onAxisMoved, (void *) 0x5);
    Gamepad_init();

    //printf("works initGamepad()\n");
}


joystickObject::joystickObject()
{
    initGamepad();
    timer.start(10);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
}
void joystickObject::timerEvent()
 {


        static unsigned int iterationsToNextPoll = 100;

        iterationsToNextPoll--;
        if (iterationsToNextPoll == 0) {
            Gamepad_detectDevices();
            iterationsToNextPoll = 100;
        }

        Gamepad_processEvents();



        if(joy.deviceAttachedorRemoved){
            emit joystickFound(Gamepad_numDevices());
            joy.deviceAttachedorRemoved=false;
        }

}

