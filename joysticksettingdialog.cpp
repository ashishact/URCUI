#include "joysticksettingdialog.h"
#include "ui_joysticksettingdialog.h"


QT_USE_NAMESPACE

JoystickSettingDialog::JoystickSettingDialog(QWidget *parent, Datas::JoySetting *joysettings, JoyData *j):
    QDialog(parent)
{
    ui = new Ui::JoystickSettingDialog;
    ui->setupUi(this);
    this->joyS = joysettings;
    this->tempjoyS = *joysettings;
    this->joydata = j;

    additionalui();
    fillDatas();
    initConnections();
}

JoystickSettingDialog::~JoystickSettingDialog()
{
}
void JoystickSettingDialog::additionalui(){
    //***********************************************

    QSizePolicy *sizepolicy;

    for(int i =0; i < 12; i++){
        if(i<6){
            horizontalLayout[i] = new QHBoxLayout();
            label[i] = new QLabel(this);
            label[i]->setText("Axis " + QString::number(i));

            horizontalLayout[i]->addWidget(label[i]);

            progressbar[i] = new QProgressBar(this);
            sizepolicy = new QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            progressbar[i]->setSizePolicy(*sizepolicy);
            progressbar[i]->setValue(50);

            horizontalLayout[i]->addWidget(progressbar[i]);

            combobox[i] = new QComboBox(this);
            sizepolicy = new QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
            combobox[i]->setSizePolicy(*sizepolicy);

            horizontalLayout[i]->addWidget(combobox[i]);

            ui->AxisverticalLayout->addLayout(horizontalLayout[i]);
        }

        if(i>5){
            horizontalLayout[i] = new QHBoxLayout();
            label[i] = new QLabel(this);
            label[i]->setText("Button " + QString::number(i-6));

            horizontalLayout[i]->addWidget(label[i]);

            progressbar[i] = new QProgressBar(this);
            sizepolicy = new QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
            progressbar[i]->setSizePolicy(*sizepolicy);
            progressbar[i]->setMinimum(0);
            progressbar[i]->setMaximum(1);
            progressbar[i]->setValue(0);
            progressbar[i]->setFormat("");

            horizontalLayout[i]->addWidget(progressbar[i]);

            combobox[i] = new QComboBox(this);
            sizepolicy = new QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
            combobox[i]->setSizePolicy(*sizepolicy);

            horizontalLayout[i]->addWidget(combobox[i]);

            ui->ButtonverticalLayout->addLayout(horizontalLayout[i]);
        }
     }
}
void JoystickSettingDialog::fillDatas(){

//    int axisSettingArray[] = {
//                                joyS->throttleAxis, joyS->turnAxis, joyS->armAct1_Axis, joyS->armAct2_Axis, joyS->armAct3_Axis,
//                             };
//    ui->axisComboBox0->addItem("throttle Axis",joyS->throttleAxis);
//    ui->axisComboBox0->addItem("turn Axis",joyS->turnAxis);
//    ui->axisComboBox0->addItem("armAct1_Axis",joyS->armAct1_Axis);
//    ui->axisComboBox0->addItem("armAct1_Axis",joyS->armAct1_Axis);


}
void JoystickSettingDialog::initConnections(){

}
void JoystickSettingDialog::joyEvent(unsigned int deviceID, bool btnEvent,unsigned int axisNo){ // Changed Axis No 0 - 6

    if(btnEvent){
        for(int i = 0; i < 6; i++){
            if(joydata->button[deviceID][i]){
                progressbar[i+6]->setValue(1);
            }
            else{
                progressbar[i+6]->setValue(0);
            }
        }
    }
    else{
        float axisValue = joydata->axis[deviceID][axisNo];

        if(axisNo < 6){
            progressbar[axisNo]->setValue((axisValue+1)*50);
        }

    }
}
void JoystickSettingDialog::joystickFound(int n){
    if(n < 1){
        ui->statusLabel->setText("No Joystick Found:  Plug in a USB Joystick");
    }
    else{
        ui->statusLabel->setText("Joystick Found");
    }
}

void JoystickSettingDialog::apply(){
    updateSettings();
    hide();
}


void JoystickSettingDialog::updateSettings(){

}
