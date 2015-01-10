#ifndef DATAS_H
#define DATAS_H

#include <QVector>
class Datas
{
public:
    struct MainCamera{
        int Pitch;
        int yaw;

        bool sendCamCommand;

        int KeyPitch_Up;
        int KeyPitch_Down;
        int KeyYaw_Up;
        int KeyYaw_Down;
    };
    struct Battery{
        qint64 lifeRemainingInMin;
        int health; // 0  - 255
    };
    struct Xbee{
        int baudRateIndex;
    };
    struct SpectroMeter{

    };
    struct SoilCollection_Assembly{
        unsigned int rotateDir;
    };
    struct Bio{
        SpectroMeter spectrometer;
        SoilCollection_Assembly soilcollection_assembly;
    };
    struct GPS{
        float lat;
        float lon;
        float alt;
    };
    struct IMU{
        float accX;
        float accY;
        float accZ;

        float velX;
        float velY;
        float velZ;

        float yaw;
        float pitch;
        float roll;

        float altitude;
    };

    struct RoboArm{
        int DOF;
        float x;
        float y;
        float depth;
        unsigned int act1Direction, act2Direction, act3Direction;
        unsigned char digger;
        unsigned char gripperAct1, gripperAct2;




    };
    struct MotorDriver{
        unsigned int throttle;
        unsigned int turnAngle;
    };

    struct JoySetting{
        int throttleAxis, turnAxis,
//            armAct1_Positive_Btn, armAct2_Positive_Btn, armAct3_Positive_Btn,
//            armAct1_Negetive_Btn, armAct2_Negetive_Btn, armAct3_Negetive_Btn,
            armAct1_Axis, armAct2_Axis, armAct3_Axis,

            armDigger_Positive_Btn, armDigger_Negetive_Btn,// armDiggerAxis,
            armGripper_Act_1_Positive_Btn, armGripper_Act_1_Negetive_Btn,
            armGripper_Act_2_Positive_Btn, armGripper_Act_2_Negetive_Btn,

            soilColl_Assembly_Rotate_Povitive_Btn, soilColl_Assembly_Rotate_Negetive_Btn;
//            soilColl_Assembly_Rotate_Axis;
    };
    struct Rover{
        Battery battery;
        MainCamera maincamera;
        Bio bio;
        GPS gps;
        IMU imu;
        MotorDriver motordriver;
        RoboArm roboarm;
        JoySetting joysetting;
        Xbee xbee;
    }rover;
public:

    struct URC{

    };
public:
    Datas();
    ~Datas();
};

#endif // DATAS_H
