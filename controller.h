#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
public:
    Controller();
    void sendControlSignalToRover(int throttle, int angle){}
    void sendControlSignalToArm(int x, int y, int depth){}
    void sendControlSignalToBioInstrument(){}
    void gotSignalFromRover(){}
    void gotSignalFromImu(){}
};

#endif // CONTROLLER_H
