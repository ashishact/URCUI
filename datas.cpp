#include "datas.h"

Datas::Datas()
{

    rover.gps.lat = 18.9; //  Put Utah value
    rover.gps.lon = 79.8; //  Put Utah value

    rover.imu.accX = 0.0;
    rover.imu.accY = 0.0;
    rover.imu.accZ = 0.0;
    rover.imu.velX = 0.0;
    rover.imu.velY = 0.0;
    rover.imu.velZ = 0.0;
    rover.imu.yaw = 0.0;
    rover.imu.pitch = 0.0;
    rover.imu.roll = 0.0;
    rover.imu.altitude = 0.0;

    rover.motordriver.throttle = 0;
    rover.motordriver.turnAngle = 0;

    rover.roboarm.x = 0.0;
    rover.roboarm.y = 0.0;
    rover.roboarm.depth = 0.0;  // depth at witch the end effector is
    rover.roboarm.DOF = 3;
    rover.roboarm.act1Direction = 0;
    rover.roboarm.act2Direction = 0;
    rover.roboarm.act3Direction = 0;
    rover.roboarm.digger =0;

    rover.roboarm.gripperAct1 = 0;
    rover.roboarm.gripperAct2 = 0;


    rover.battery.lifeRemainingInMin = 10000;  // a value greator than the possible greatest life time of the Battery
    rover.battery.health = 255;   // 255 for 100%

    rover.bio.soilcollection_assembly.rotateDir = 0;

    rover.maincamera.Pitch =0;
    rover.maincamera.yaw = 0;
    rover.maincamera.sendCamCommand = false;
    rover.maincamera.KeyPitch_Up = 87;
    rover.maincamera.KeyPitch_Down = 83;
    rover.maincamera.KeyYaw_Up = 68;
    rover.maincamera.KeyYaw_Down = 65;

    rover.joysetting.throttleAxis = 1;
    rover.joysetting.turnAxis = 2;

    rover.joysetting.armAct1_Axis = 4;
    rover.joysetting.armAct2_Axis = 5;
    rover.joysetting.armAct3_Axis = 3;

    rover.joysetting.armDigger_Positive_Btn = 4;
    rover.joysetting.armDigger_Negetive_Btn = 5;
    rover.joysetting.armGripper_Act_1_Positive_Btn = 0;
    rover.joysetting.armGripper_Act_1_Negetive_Btn = 2;
    rover.joysetting.armGripper_Act_2_Positive_Btn = 3;
    rover.joysetting.armGripper_Act_2_Negetive_Btn = 1;

    rover.joysetting.soilColl_Assembly_Rotate_Povitive_Btn = 0;
    rover.joysetting.soilColl_Assembly_Rotate_Negetive_Btn = 2;
    /*
    rover.joysetting.armAct1_Positive_Btn = 3;
    rover.joysetting.armAct1_Negetive_Btn = 1;
    rover.joysetting.armAct2_Positive_Btn = 0;
    rover.joysetting.armAct2_Negetive_Btn = 2;
    rover.joysetting.armAct3_Positive_Btn = 0;
    rover.joysetting.armAct3_Negetive_Btn = 2;*/
//    rover.joysetting.armDiggerAxis = -1;
//    rover.joysetting.soilColl_Assembly_Rotate_Axis = 5;

    rover.xbee.baudRateIndex = 0;




}


Datas::~Datas()
{

}
