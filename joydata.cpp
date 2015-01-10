#include "joydata.h"

JoyData::JoyData()
{
    for (int i=0; i<MaxNoButton; i++){
            button[0][i]=false; // 1st joystick
            button[1][i]=false;
        }
    for(int i=0;i<MaxNoAxis;i++){
        axis[0][i]=0;
        axis[1][i]=0;
    }
//    device=0;
//    changedAxisNo=0;
//    btnEv=false;
    deviceAttachedorRemoved = true;
}
