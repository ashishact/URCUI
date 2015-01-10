#ifndef DATAMANIPULATOR_H
#define DATAMANIPULATOR_H

#include "datas.h"
class DataManipulator
{
    Datas *datas;
public:
    DataManipulator(Datas *data);
    void setGPSData(){}
    void getGPSData(){}
    void setIMUData(){}
    void getIMUData(){}
    void setRoboArmData(){}
    void getRoboArmData(){}
    void setBioData(){}
    void getBioData(){}
    void setCameraData(){}
    void getCameraData(){}
    void setBatteryData(){}
    void getBatteryData(){}
    void setRoverData(){}
    void getRoverData(){}
};

#endif // DATAMANIPULATOR_H
