#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_urc_controlwidget.h"  // for controls and command .ui File
#include "ui_urc_terminalwidget.h"  // for terminal .ui File


#include "Library/qcustomplot/qcustomplot.h"

#include "settingsdialog.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <qmath.h>

#include <gamepad/Gamepad.h>

#include "joystickobject.h"
#include "joystickobject.cpp"
#include "map/ImageCache.h"

#include <plot/urc_plot.h>
#include "roverview/urc_roverview.h"

#define DELIMETER 251
#define MAXBVALUE 250

#define USE_TCP_SERVER 0

#define BLACKBOX_ENABLE 1

//#include <QDebug>

/**
 * @brief MainWindow::MainWindow
 * @param parent
 *
 *  Constructor for The MainWindow, as the name suggests, it is the main window that runs the program
 *
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    maxcomportdatasize = 40;

    m_settingsFile = QString("config/config.ini");
    roverdata=new Datas();

    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    joysticksettingdialog = new JoystickSettingDialog(this, &roverdata->rover.joysetting, &joy);

    ui->setupUi(this);

    m_timer.start(10);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
//tcp server
#if USE_TCP_SERVER
    tcpServer = new QTcpServer(this);
    if(true == tcpServer->listen(QHostAddress("127.0.0.1"), 9000)){
        //ui->statusBar->showMessage(tr("listening on port 9000 . Server Created Open blender program"));
        qDebug() << "listening on port 9000";
    }
    else QMessageBox::critical(this,tr("TCP Server Error"),tr("Something is wrong with your machine"));


    clientConnection = NULL;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(handleClientConnection()));
#endif
//ini file

    initActionsConnections();
    initqmapwidget();
    createDockWindows();
    initRest();
    resetAll();

    loadSettingFile();

}
/**
 * @brief MainWindow::~MainWindow
 *
 *  Destructor
 */
MainWindow::~MainWindow()
{
    saveToSettingFile();
    this->saveGPSPathstoFile();


    if(control_ui_tw)delete control_ui_tw;
    if(serial->isOpen()){
        serial->close();
        qDebug()<<"serialport forced closed";
    }
#if USE_TCP_SERVER
    if(clientConnection != NULL && clientConnection->isOpen()){
        clientConnection->close();
    }
    if(tcpServer->isListening()){
        tcpServer->close();
    }
    delete tcpServer;
    delete clientConnection;
#endif

    delete serial;

    delete settings;


    delete roverdata;
    delete m_MapWidget;
    delete ui;
}
void MainWindow::timerEvent()
{

    QByteArray data;
    QString temp="";
    //***************************// rover drive
    if(control_ui->driveEncodingDecisionCheckBox->isChecked()){
        data = encodeData(0);
        if(serial->isOpen()){
            writeData(data);
        }




        if(terminal_ui->terminalDriveCheckBox->isChecked()){
            temp = "Rover Drive-->";
            for(int i=0; i < data.size();i++){
                temp.append(QString::number((byte)data.at(i)));
                temp.append(" : ");
            }
            terminalString.append(temp);
            printTerminalString();

        }
    }
//    else{
//        if(serial->isOpen()){
//            data.append(251);
//            writeData(data);
//        }
//    }
    //***************************// arm
    if(control_ui->armControlEncodingDecisioncheckBox->isChecked()){
//        if(roverdata->rover.roboarm.act1Direction
//                || roverdata->rover.roboarm.act2Direction
//                || roverdata->rover.roboarm.act3Direction){

            data = encodeData(1);
            if(serial->isOpen()){
                writeData(data);
            }

            if(terminal_ui->terminalArmCheckBox->isChecked()){
                temp = " Arm -> ";
                for(int i=0; i < data.size();i++){
                    temp.append(QString::number((byte)data.at(i)));
                    temp.append(" : ");
                }
                terminalString.append(temp);
                printTerminalString();
            }
//        }
    }
    //***************************// camera
    //if(roverdata->rover.maincamera.sendCamCommand){
    if(control_ui->mainCameraControlDecisionCheckBox->isChecked()){

        data = encodeData(2);
        if(serial->isOpen()){
            writeData(data);
        }
        temp = " Main Camera -> ";
        for(int i=0; i < data.size();i++){
            temp.append(QString::number((byte)data.at(i)));
            temp.append(" : ");
        }
        terminalString.append(temp);
        printTerminalString();

    }
    //********************************// soil collection
    if(control_ui->EnableSoilCollectioncheckBox->isChecked()){

        data = encodeData(5);
        if(serial->isOpen()){
            writeData(data);
        }
        temp = " Soil Collection -> ";
        for(int i=0; i < data.size();i++){
            temp.append(QString::number((byte)data.at(i)));
            temp.append(" : ");
        }
        terminalString.append(temp);
        printTerminalString();

    }
    //***************************//

//         static int ang =30;
//         m_MapWidget->setPixmapHeading(ang);
//         ang++;
//         if(ang>356){
//            ang=0;
//            float la = qrand()%30;
//            float lo = qrand()%20;
//            updateGPSValues(la,lo,1);
//         }

//    static int r = 0;r++;if(r>250)r=0;
//    updateIMUValues(r,r+100,r);

}

/**
 * @brief MainWindow::openSerialPort
 *
 *  It opens the serial port based on the setting Structure, which is defined in SettingsDialog class
 *
 */

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    if (serial->open(QIODevice::ReadWrite)) {
        if (serial->setBaudRate(p.baudRate)
                && serial->setDataBits(p.dataBits)
                && serial->setParity(p.parity)
                && serial->setStopBits(p.stopBits)
                && serial->setFlowControl(p.flowControl)) {


            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
            ui->importanatStatusLabel->setText("Connected :-)");
        } else {
            serial->close();
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
        ui->importanatStatusLabel->setText("Error !");
    }
}

/**
 * @brief MainWindow::closeSerialport
 *  Closes the serial port
 */
void MainWindow::closeSerialPort()
{
    serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
    ui->importanatStatusLabel->setText("Xbee Not Connected");
}

/*
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("<b>MSIUI </b> is  "
                          "designed to control rover "
                          "especially for Mars Society India"));
}*/


/**
 * @brief MainWindow::writeData
 * @param data
 *
 *write data to
 */
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);


    if(terminal_ui->terminalOutcheckBox->isChecked()){
        QString temp ="";

        for(int i=0; i < data.size();i++){
            temp.append(QString::number((byte)data.at(i)));
            temp.append(" : ");
        }
        terminalString.append(temp);
        printTerminalString();
    }
}
void MainWindow::readData()
{
    QByteArray data = serial->readAll();

//    urc_plot->URC_vel_data_update(int(data[0]),int(data[0]) + 12);
//    urc_roverview->updateroll(int(data[0]));


    comportdata.append(data);

    if(terminal_ui->terminalIncheckBox->isChecked()){
        QString temp = "Incoming Data -> ";
        for(int i=0; i < data.size();i++){
//            temp.append(QString::number((byte)data.at(i)));
//            temp.append(" : ");
            temp.append(data.at(i));
        }
        terminalString.append(temp);
        printTerminalString();
    }
#if USE_TCP_SERVER
//    if(clientConnection != NULL){
//        clientConnection->write(data);
//    }// this is needed if  you want to send all these datas to a client on this machine
#endif

    emit this->decodeData();
}

QByteArray MainWindow::encodeData(const int type)
{
    QByteArray data;
    switch(type)
    {
        //***************************************************************
        case 0: // Motor Driver
            data.append(DELIMETER);
            data.append(181);
            data.append(roverdata->rover.motordriver.throttle);
            data.append(roverdata->rover.motordriver.turnAngle);
            break;
        //***************************************************************
        case 1: // Arm
            data.append(DELIMETER);
            data.append(191);// identifier
            data.append(roverdata->rover.roboarm.act1Direction);
            data.append(roverdata->rover.roboarm.act2Direction);
            data.append(roverdata->rover.roboarm.act3Direction);
            data.append(roverdata->rover.roboarm.digger);
            data.append(roverdata->rover.roboarm.gripperAct1);
            data.append(roverdata->rover.roboarm.gripperAct2);
            break;
        //****************************************************************
        case 2: // Camera
            data.append(DELIMETER);
            data.append(192);//camera identifier
            data.append(roverdata->rover.maincamera.Pitch);
            data.append(roverdata->rover.maincamera.yaw);
            break;
        //***************************************************************
        case 3: //  Bio Start Soil collection (Automation)
            data.append(DELIMETER);
            data.append(200);// bio identifier
            data.append(11);    // soil collection
            break;
        case 4: //  Bio Start Experiment(Automation)
            data.append(DELIMETER);
            data.append(200);// bio identifier
            data.append(21);    // bio exp start
            break;
        case 5: // Soil collection
            data.append(DELIMETER);
            data.append(200);// bio identifier
            data.append(roverdata->rover.bio.soilcollection_assembly.rotateDir);
            break;
        //***************************************************************

    }
    return data;
}
void MainWindow::decodeData()
{
#if !BLACKBOX_ENABLE
    int gpsdatalength = 10;// identifier + data
    int imudatalength = 4;
    int biodatalength = 2;
    for(int i =0; i < comportdata.size(); i++){
        if((unsigned char)comportdata.at(i) == DELIMETER){
            //qDebug()<<"Delemeter found";
            i++;
            unsigned char dataID_L = (unsigned char)comportdata.at(i);

            if(dataID_L == 'G'){
               // qDebug()<<"gps id found";
                if(comportdata.size() - i >= gpsdatalength+1 &&
                        (unsigned char)comportdata.at(i+gpsdatalength) == DELIMETER){
                    double lat = (unsigned char)comportdata.at(i+1) +
                            (unsigned char)comportdata.at(i+2)*0.01 +
                            (unsigned char)comportdata.at(i+3)*0.0001 +
                            (unsigned char)comportdata.at(i+4)*0.000001;
                    double lon = (unsigned char)comportdata.at(i+5) +
                            (unsigned char)comportdata.at(i+6)*0.01 +
                            (unsigned char)comportdata.at(i+7)*0.0001 +
                            (unsigned char)comportdata.at(i+8)*0.000001;
                    int region = (unsigned char)comportdata.at(i+9);
                    if(lon > 180){
                        QString temp="";
                        for(int i=0; i < comportdata.size();i++){
                            temp.append(QString::number(i)+" -> "+QString::number((byte)comportdata.at(i)));
                            temp.append(" \n ");
                        }
                       qDebug()<<temp;
                       qDebug()<<"lon :  "<<lon;
                       qDebug()<<"i is :  "<<i;
                       qDebug()<<"at i  we have : "<<(unsigned char)comportdata.at(i);
                       qDebug()<<"at i+5  we have : "<<(unsigned char)comportdata.at(i+5);
                    }
                    updateGPSValues(lat,lon,region);
                    i = i+gpsdatalength;
//                    if(i< comportdata.size()){qDebug()<<(unsigned char)comportdata.at(i);}
                }
            }
            else if(dataID_L == 'I'){
                //qDebug()<<"IMU id found";
                if(comportdata.size() - i >= imudatalength+1 &&
                        (unsigned char)comportdata.at(i+imudatalength) == DELIMETER){
                    int roll = (unsigned char)comportdata.at(i+1);
                    int pitch = (unsigned char)comportdata.at(i+2);
                    int heading = (unsigned char)comportdata.at(i+3)*2 ;
                    updateIMUValues(roll,pitch,heading);

                    i = i+imudatalength;
                }
            }
            else if(dataID_L == 'B'){
                if(comportdata.size() - i >= biodatalength+1 &&
                        (unsigned char)comportdata.at(i+biodatalength) == DELIMETER){
                    unsigned char val = (unsigned char)comportdata.at(i+1);
                    if(val == 11){
                        control_ui->soilCollStatusLabel->setText(" Started and Running");
                    }
                    else if(val == 12){
                        control_ui->soilCollStatusLabel->setText(" Finished Successfully ");
                    }
                    else if(val == 21){
                        control_ui->bioExpStatusLabel->setText(" Started and Running");
                    }
                    else if(val == 22){
                        control_ui->bioExpStatusLabel->setText(" Finished Successfully ");
                    }
                    i = i+biodatalength;
                }
            }
            else{
                qDebug()<<"251 found but no Identifier found";
            }
        }//check DELIMETER

    }//for end
    if(comportdata.size()>maxcomportdatasize){
        comportdata.remove(0,comportdata.size()-maxcomportdatasize);
    }


#endif

#if BLACKBOX_ENABLE
    QList<QByteArray> dollar = comportdata.split('$');
    for(int i = 0; i < dollar.size(); i++){
//        qDebug()<<"Dollar "<<dollar.at(i);
        QList<QByteArray> colon = dollar[i].split(':');
        if(colon.size() >= 2){
//            qDebug()<<colon.at(0);
//            qDebug()<<colon.at(1);
            if(colon.at(0).toInt() == 2){
                QList<QByteArray> arduIMUDatas = colon.at(1).split(',');
                if(arduIMUDatas.size() == 3){
                    float roll = arduIMUDatas.at(0).toFloat();
                    float pitch = arduIMUDatas.at(1).toFloat();
                    float yaw = arduIMUDatas.at(2).toFloat();

                    updateIMUValues(roll,pitch,yaw);
                }
//                else qDebug()<<"error in arduimu datas";
            }
            else if(colon.at(0).toInt() == 3){
                QList<QByteArray> MAGDatas = colon.at(1).split(',');
                if(MAGDatas.size() == 3){
                    float MAGX = MAGDatas.at(0).toFloat();
                    float MAGY = MAGDatas.at(1).toFloat();
                    float MAGZ = MAGDatas.at(2).toFloat();
                    m_MapWidget->setPixmapHeading(MAGX);
                }
//                else qDebug()<<"error in Magnetometer datas";
            }
            else if(colon.at(0).toInt() == 4){
                    QList<QByteArray> GPSDatas = colon.at(1).split(',');
                    if(GPSDatas.size() == 3){
                        float LAT = GPSDatas.at(0).toFloat();
                        float LON = GPSDatas.at(1).toFloat();
                        float NO = GPSDatas.at(2).toFloat();
                        updateGPSValues(LAT,LON,0);
                    }
//                    else qDebug()<<"error in GPS datas";

            }
        }
    }
//    int lidx = comportdata.lastIndexOf('$')-1;
//    if(lidx > 0 && lidx < comportdata.size())comportdata.remove(0,lidx);
    if(comportdata.size()>maxcomportdatasize){
        comportdata.remove(0,comportdata.size()-maxcomportdatasize);
    }

#endif

}
void MainWindow::updateGPSValues(double lat,double lon, int region){
    //roverdata->GPS.lat = lat;
    //roverdata->GPS.lon = lon;
    control_ui->LatValueLabel->setText(QString::number(lat,'g',10));
    control_ui->LongValueLabel->setText(QString::number(lon,'g',10));
    m_MapWidget->updateGPSValues(lat,lon);

}
void MainWindow::updateIMUValues(int roll, int pitch, int heading){
    control_ui->RollValueLabel->setText(QString::number(roll));
    control_ui->PitcValueLabel->setText(QString::number(pitch));
    control_ui->HeadingValueLabel->setText(QString::number(heading));

    urc_roverview->updateroll(roll);
    urc_roverview->updatepitch(pitch);

    QByteArray data;
    data.append(DELIMETER);
    data.append(roll);
    data.append(pitch);
    data.append(heading/2);
#if USE_TCP_SERVER
    if(clientConnection != NULL){
        clientConnection->write(data);
    }
#endif

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
        ui->importanatStatusLabel->setText("Error !!");
    }
}

void MainWindow::initActionsConnections()
{
    connect(&joy,SIGNAL(changed(unsigned int, bool,unsigned int)), this, SLOT(joyEvent(unsigned int, bool, unsigned int)));
    connect(&m_joy,SIGNAL(joystickFound(int)),this,SLOT(joystickFound(int)));
    connect(&m_joy,SIGNAL(joystickFound(int)),joysticksettingdialog,SLOT(joystickFound(int)) );


    // Defaul in out port
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()) );
    connect(ui->actionJoyStick1, SIGNAL(triggered()), joysticksettingdialog , SLOT(show()));


    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(ui->actionOpenTerminal,SIGNAL(triggered()),this,SLOT(toggleTerminal()));
    connect(ui->actionSaveGPSpathsToFile,SIGNAL(triggered()),this,SLOT(saveGPSPathstoFile()));
   // connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    //connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    //connect(ui->actionAppSettings,SIGNAL(triggered()),this,SLOT());
    connect(this,SIGNAL(decodecomportdatasig()),this,SLOT(decodeData()));
}
void MainWindow::initRest(){

}

void MainWindow::joyEvent(unsigned int deviceID, bool btnEvent,unsigned int axisNo){ // Changed Axis No 0 - 6

    float axisValue = joy.axis[deviceID][axisNo];
    Datas::JoySetting *joyS = &(roverdata->rover.joysetting);
#define ARMACTUATOR_USE_BTN 0
#define DIGGER_USE_BTN 1
    if(deviceID == 0){// 1st joystick

        if(btnEvent){
//************************************************************//
#if ARMACTUATOR_USE_BTN
            // actuator 1
            if(joy.button[0][joyS->armAct1_Positive_Btn]){   // button 3
                roverdata->rover.roboarm.act1Direction = 19;   // act1 +
            }
            else if(joy.button[0][joyS->armAct1_Negetive_Btn]){   // device 1 button 0
                roverdata->rover.roboarm.act1Direction = 11;   // act1 -
            }
            else {
                roverdata->rover.roboarm.act1Direction = 0;   // act1 no Movement // So don't send anything
            }

            // actuator 2
            if(joy.button[0][joyS->armAct2_Positive_Btn]){   // device 1 button 1
                roverdata->rover.roboarm.act2Direction = 29;   // act2 +
            }
            else if(joy.button[0][joyS->armAct2_Negetive_Btn]){   // device 1 button 2
                roverdata->rover.roboarm.act2Direction = 21;   // act2 -
            }
            else {
                roverdata->rover.roboarm.act2Direction = 0;   // act2 no Movement // So don't send anything
            }

//******************************************//
            // actuator 3
            if(joy.button[0][joyS->armAct3_Positive_Btn]){   // device 1 button 4
                roverdata->rover.roboarm.act3Direction = 39;   // act3 +
            }
            else if(joy.button[0][joyS->armAct3_Negetive_Btn]){   // device 1 button 5
                roverdata->rover.roboarm.act3Direction = 31;   // act3 -
            }
            else {
                roverdata->rover.roboarm.act3Direction = 0;   // act3 no Movement // So don't send anything
            }
#endif
//********************************************//
#if DIGGER_USE_BTN
            if(joy.button[0][joyS->armDigger_Positive_Btn]){
                roverdata->rover.roboarm.digger = 49;
            }
            else if(joy.button[0][joyS->armDigger_Negetive_Btn]){
                roverdata->rover.roboarm.digger = 41;
            }
            else roverdata->rover.roboarm.digger = 0;
#endif
//*****************************************// gripper act 1
            if(joy.button[0][joyS->armGripper_Act_1_Positive_Btn]){
                roverdata->rover.roboarm.gripperAct1 = 59;
            }
            else if(joy.button[0][joyS->armGripper_Act_1_Negetive_Btn]){
                roverdata->rover.roboarm.gripperAct1 = 51;
            }
            else{
                roverdata->rover.roboarm.gripperAct1 = 0;
            }
            if(joy.button[0][joyS->armGripper_Act_2_Positive_Btn]){// gripper act 2
                roverdata->rover.roboarm.gripperAct2 = 69;
            }
            else if(joy.button[0][joyS->armGripper_Act_2_Negetive_Btn]){
                roverdata->rover.roboarm.gripperAct2 = 61;
            }
            else{
                roverdata->rover.roboarm.gripperAct2 = 0;
            }
//***********************************************// Soil collection assembly
            if(joy.button[0][joyS->soilColl_Assembly_Rotate_Povitive_Btn]){
                roverdata->rover.bio.soilcollection_assembly.rotateDir = 39;
            }
            else if(joy.button[0][joyS->soilColl_Assembly_Rotate_Negetive_Btn]){
                roverdata->rover.bio.soilcollection_assembly.rotateDir = 31;
            }
        }
//************************************************//
        else{   // axis
            if(axisNo == joyS->throttleAxis){
                if(axisValue < 0)  // forward
                    roverdata->rover.motordriver.throttle = 1 - axisValue*(MAXBVALUE/2 - 1);
                else                // backward
                    roverdata->rover.motordriver.throttle = MAXBVALUE/2 + 1 + axisValue*(MAXBVALUE/2 - 1);
            }
            else if(axisNo == joyS->turnAxis){
                roverdata->rover.motordriver.turnAngle =1 + ( (axisValue+1)/2 )*249;
            }
//***************************************************//
#if DIGGER_USE_BTN == 0
            if(axisNo == joyS->armDiggerAxis){
                if(axisValue == -1)roverdata->rover.roboarm.digger = 41;
                else if(axisValue == 1)roverdata->rover.roboarm.digger = 49;
                else roverdata->rover.roboarm.digger = 0;
            }
#endif
//*************************************************//
#if ARMACTUATOR_USE_BTN == 0
            if(axisNo == joyS->armAct1_Axis){
                if(axisValue < -0.8)roverdata->rover.roboarm.act1Direction = 19;
                else if(axisValue > 0.8)roverdata->rover.roboarm.act1Direction = 11;
                else roverdata->rover.roboarm.act1Direction = 0;
            }
            if(axisNo == joyS->armAct2_Axis){
                if(axisValue > 0.8)roverdata->rover.roboarm.act2Direction = 29;
                else if(axisValue < -0.8)roverdata->rover.roboarm.act2Direction = 21;
                else roverdata->rover.roboarm.act2Direction = 0;
            }
            if(axisNo == joyS->armAct3_Axis){
                if(axisValue > 0.8)roverdata->rover.roboarm.act3Direction = 39;
                else if(axisValue < -0.8)roverdata->rover.roboarm.act3Direction = 31;
                else roverdata->rover.roboarm.act3Direction = 0;
            }
#endif
//*********************************************//
        }
    }
    else if(deviceID == 1){

    }

    if(terminal_ui->terminalJoycheckBox->isChecked()){
        if(btnEvent){
            int btn;
            for(int i=0; i< MaxNoButton; i++){
                if(joy.button[deviceID][i]){
                    btn=i;
                    terminalString.append("Button : "+QString::number(btn)+"  Pressed with joystick ID: "+QString::number(deviceID));
                 }
            }
        }
        else{

            terminalString.append("Axis : "+QString::number(axisNo)+"  Changed to  : "+ QString::number(joy.axis[deviceID][axisNo]) + " #with ID: "+QString::number(deviceID));
        }
        printTerminalString();
    }
    if(joysticksettingdialog->isActiveWindow()){
        joysticksettingdialog->joyEvent(deviceID,btnEvent,axisNo);
    }
}


void MainWindow:: printTerminalString(){
    QString s;
    int maxStringSize =12;
    for (int i = 0; i < terminalString.size(); ++i) {

        s+= terminalString.at(i)+"\n";
    }
    terminal_ui->terminalUI->setPlainText(s);
    if(terminalString.size() > maxStringSize){
        terminalString.erase(terminalString.begin(), terminalString.end()-maxStringSize);
    }
}
void MainWindow::joystickFound(int n){
    if(n < 1){
        ui->actionJoyStick1->setEnabled(false);

        ui->statusBar->showMessage(tr("No Joystick Found:  Plug in a USB Joystick"));
        ui->importanatStatusLabel->setText("No Joystick Found:");
    }
    else{
        ui->actionJoyStick1->setEnabled(true);
        ui->statusBar->showMessage(tr("Joystick Found: Make sure Analog Button is On"));
        ui->importanatStatusLabel->setText("Joystick Found ");
    }
}
void MainWindow::initqmapwidget(){

        QObject::connect(ui->actionCenterSpot, SIGNAL(triggered()), this, SLOT(centerMapToSpot()));
#if followTarget
        QObject::connect(ui->actionFollowSpot, SIGNAL(triggered()), this, SLOT(followSpot()));
#endif

        m_MapWidget = new QMapWidget();
        m_MapWidget->setCache(&imageCache);

        mapzoomsslider = new QSlider(Qt::Vertical,this);
        mapzoomsslider->setRange(1,19);
        mapzoomsslider->connect(m_MapWidget,SIGNAL(zoomLevelChanged(int)),mapzoomsslider,SLOT(setValue(int)));
        mapzoomsslider->connect(mapzoomsslider,SIGNAL(valueChanged(int)),m_MapWidget,SLOT(setZoomLevel(int)));


        QHBoxLayout *layout = new QHBoxLayout;
        QVBoxLayout *vlay = new QVBoxLayout;
        QLabel *zoomlabel = new QLabel("1");

        zoomlabel->connect(m_MapWidget,SIGNAL(zoomLevelChanged(int)),zoomlabel,SLOT(setNum(int)));

        vlay->setContentsMargins(0,0,0,0);

        vlay->addWidget(zoomlabel);
        vlay->addWidget(mapzoomsslider);

        layout->addWidget(m_MapWidget);
        layout->addLayout(vlay);
        layout->setContentsMargins(0,0,0,0);

        QWidget *map = new QWidget();
        map->setLayout(layout);

        this->setCentralWidget(map);

        ui->mainToolBar->addSeparator();
        ui->mainToolBar->addWidget(ui->importanatStatusLabel);
        ui->mainToolBar->addSeparator();
        ui->mainToolBar->addWidget(ui->openLoopControlStatusLabel);


}
void MainWindow::centerMapToSpot()
{
    m_MapWidget->centerToSpot();

}

void MainWindow::resetAll(){
    terminal_ui->terminalUI->setPlainText("--- Messages--- ");
    terminalString.clear();
    centerMapToSpot();

}
void MainWindow::toggleTerminal()
{
    if(terminal_ui->terminalJoycheckBox->isChecked() || terminal_ui->terminalIncheckBox->isChecked()
            || terminal_ui->terminalOutcheckBox->isChecked()){

        terminal_ui->terminalIncheckBox->setCheckState(Qt::Unchecked);
        terminal_ui->terminalJoycheckBox->setCheckState(Qt::Unchecked);
        terminal_ui->terminalOutcheckBox->setCheckState(Qt::Unchecked);

        terminal_ui->terminalUI->setPlainText("----Terminal Stopped------\n\n Press Ctrl+T to Start again\n");
        terminalString.clear();
    }
    else {
        terminal_ui->terminalIncheckBox->setCheckState(Qt::Checked);
        terminal_ui->terminalJoycheckBox->setCheckState(Qt::Checked);
        terminal_ui->terminalOutcheckBox->setCheckState(Qt::Checked);

        terminal_ui->terminalUI->setPlainText("------Terminal Started------\n");
    }
}


void MainWindow::stopRover()
{
    QByteArray d;
    d.append(DELIMETER);
    d.append(250);
    if(serial->isOpen()) writeData(d);
}

void MainWindow::stopArm()
{
    QByteArray d;
    d.append(DELIMETER);
    d.append(191);
    d.append(250);
    if(serial->isOpen()) writeData(d);
}
void MainWindow::changeCacheDirButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Change Cache Directory for Map tiles"),
                                             tr("Cache Directory    :: USE /"), QLineEdit::Normal,
                                             tr("C:/Users/Ashish Act/.GMapCatcher/sat_tiles"), &ok);
        if (ok && !text.isEmpty())
            this->m_MapWidget->images->setCacheDir(text);
}


void MainWindow::createDockWindows()
{
// controls
    QDockWidget *dock = new QDockWidget(tr("Controls"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    control_ui_tw = new QTabWidget(dock);
    control_ui = new Ui::URC_ControlWidget;
    control_ui->setupUi(control_ui_tw);

    dock->setWidget(control_ui_tw);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setMinimumSize(250,25);
    ui->menuView->addAction(dock->toggleViewAction());

// Terminal
    dock = new QDockWidget(tr("Terminal"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    terminal_ui_w = new QWidget(dock);
    terminal_ui = new Ui::URC_TerminalWidget;
    terminal_ui->setupUi(terminal_ui_w);    
    dock->setWidget(terminal_ui_w);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setMinimumSize(250,25);
    ui->menuView->addAction(dock->toggleViewAction());


// Plooting
    dock = new QDockWidget(tr("Plot"),this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    urc_plot = new URC_Plot(this);
    urc_plot->setup_URC_vel_plot();

    dock->setWidget(urc_plot);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    dock->setMinimumSize(100,25);
    ui->menuView->addAction(dock->toggleViewAction());
    dock->setVisible(false);


// roverview
    dock = new QDockWidget(tr("RoverView"),this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    urc_roverview = new URC_RoverView(this);


    dock->setWidget(urc_roverview);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setMinimumSize(4,3);
    ui->menuView->addAction(dock->toggleViewAction());
    dock->setVisible(false);


    //*********************************

    setupAdditionalUis();

}

void MainWindow::setupAdditionalUis(){
    connect(control_ui->changeCacheDirButton,SIGNAL(clicked()),this,SLOT(changeCacheDirButton_clicked()));
    connect(control_ui->satartSoilCollPushButton,SIGNAL(clicked()),this,SLOT(satartSoilCollPushButton_clicked()));
    connect(control_ui->statrBioExpPushButton,SIGNAL(clicked()),this,SLOT(statrBioExpPushButton_clicked()));

    control_ui->mainCameraPitch_Uplabel->setText(" ++ "+ QString::number(roverdata->rover.maincamera.KeyPitch_Up));
    control_ui->mainCameraPitch_DownLabel->setText(" -- "+ QString::number(roverdata->rover.maincamera.KeyPitch_Down));
    control_ui->mainCameraYaw_UpLabel->setText(" ++ "+ QString::number(roverdata->rover.maincamera.KeyYaw_Up));
    control_ui->mainCameraYaw_DownLabel->setText(" -- "+ QString::number(roverdata->rover.maincamera.KeyYaw_Down));

}
void MainWindow::keyPressEvent(QKeyEvent *e){
    if(e->key() == roverdata->rover.maincamera.KeyPitch_Up){
        roverdata->rover.maincamera.Pitch = 19;
        roverdata->rover.maincamera.sendCamCommand = true;
        //qDebug()<<e->key();
    }
    else if(e->key() == roverdata->rover.maincamera.KeyPitch_Down){
        roverdata->rover.maincamera.Pitch = 11;
        roverdata->rover.maincamera.sendCamCommand = true;
        //qDebug()<<e->key();
    }
    else if(e->key() == roverdata->rover.maincamera.KeyYaw_Up){
        roverdata->rover.maincamera.yaw = 29;
        roverdata->rover.maincamera.sendCamCommand = true;
        //qDebug()<<e->key();
    }
    else if(e->key() == roverdata->rover.maincamera.KeyYaw_Down){
        roverdata->rover.maincamera.yaw = 21;
        roverdata->rover.maincamera.sendCamCommand = true;
        //qDebug()<<e->key();
    }

//    if(e->key() == 'V'){
//        m_MapWidget->centerToThis();
//    }

}
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == roverdata->rover.maincamera.KeyPitch_Up
            || e->key() == roverdata->rover.maincamera.KeyPitch_Down
            || e->key() == roverdata->rover.maincamera.KeyYaw_Up
            || e->key() == roverdata->rover.maincamera.KeyYaw_Down){
        roverdata->rover.maincamera.Pitch = 0;
        roverdata->rover.maincamera.yaw = 0;
        roverdata->rover.maincamera.sendCamCommand =false;
		
//		QByteArray data = encodeData(2);
//        if(serial->isOpen()){
//            writeData(data);
//        }
    }
}

void MainWindow::loadSettingFile()
{
    QSettings sett(m_settingsFile,QSettings::IniFormat,this);

    Datas::MainCamera *mcam;
    mcam= &(roverdata->rover.maincamera);
    mcam->KeyPitch_Up = sett.value("MainCamera/KeyCodePitch_Up",mcam->KeyPitch_Up).toInt();
    mcam->KeyPitch_Down = sett.value("MainCamera/KeyCodePitch_Down",mcam->KeyPitch_Down).toInt();

    Datas::JoySetting *joyS = &(roverdata->rover.joysetting);
    joyS->throttleAxis = sett.value("JOYSTICK/Throttle Axis", joyS->throttleAxis).toInt();
    joyS->turnAxis = sett.value("JOYSTICK/Turn Axis", joyS->turnAxis).toInt();

    joyS->armAct1_Axis = sett.value("JOYSTICK/ARM_Actuator_1_Axis", joyS->armAct1_Axis).toInt();
    joyS->armAct2_Axis = sett.value("JOYSTICK/ARM_Actuator_2_Axis", joyS->armAct2_Axis).toInt();
    joyS->armAct3_Axis = sett.value("JOYSTICK/ARM_Actuator_3_Axis", joyS->armAct3_Axis).toInt();

//    joyS->armAct3_Positive_Btn = sett.value("JOYSTICK/ARM_Actuator_3_Button_Positive_Dir", joyS->armAct3_Positive_Btn).toInt();
//    joyS->armAct3_Negetive_Btn = sett.value("JOYSTICK/ARM_Actuator_3_Button_Negetive_Dir", joyS->armAct3_Negetive_Btn).toInt();

    joyS->armDigger_Positive_Btn = sett.value("JOYSTICK/ARM_Digger_Button_Positive_Dir", joyS->armDigger_Positive_Btn).toInt();
    joyS->armDigger_Negetive_Btn = sett.value("JOYSTICK/ARM_Digger_Button_Negetive_Dir", joyS->armDigger_Negetive_Btn).toInt();

    joyS->armGripper_Act_1_Positive_Btn = sett.value("JOYSTICK/ARM_Gripper_Actuator_1_Pos",joyS->armGripper_Act_1_Positive_Btn).toInt();
    joyS->armGripper_Act_1_Negetive_Btn = sett.value("JOYSTICK/ARM_Gripper_Actuator_1_Neg",joyS->armGripper_Act_1_Negetive_Btn).toInt();

    joyS->armGripper_Act_2_Positive_Btn = sett.value("JOYSTICK/ARM_Gripper_Actuator_2_Pos",joyS->armGripper_Act_2_Positive_Btn).toInt();
    joyS->armGripper_Act_2_Negetive_Btn = sett.value("JOYSTICK/ARM_Gripper_Actuator_2_Neg",joyS->armGripper_Act_2_Negetive_Btn).toInt();

    joyS->soilColl_Assembly_Rotate_Povitive_Btn = sett.value("JOYSTICK/Soil_Collection_Assembly_Button_Pos_Dir",joyS->soilColl_Assembly_Rotate_Povitive_Btn).toInt();
    joyS->soilColl_Assembly_Rotate_Negetive_Btn = sett.value("JOYSTICK/Soil_Collection_Assembly_Button_Neg_Dir",joyS->soilColl_Assembly_Rotate_Negetive_Btn).toInt();

    Datas::Xbee *x = &(roverdata->rover.xbee);
    x->baudRateIndex = sett.value("SERIALPORT/BaudRateIndex",x->baudRateIndex).toInt();
    settings->setBaudRateIndex(x->baudRateIndex);

    m_MapWidget->astroPixmapSizeInPercentage = sett.value("MAP/astroPixmapSizeInPercentage",100).toInt();

}
void MainWindow::saveToSettingFile()
{
    QSettings sett(m_settingsFile,QSettings::IniFormat,this);

    sett.setValue("Org/Name","Mars Society  India");

    Datas::MainCamera mc = roverdata->rover.maincamera;
    sett.setValue("MainCamera/KeyCodePitch_Up",mc.KeyPitch_Up);
    sett.setValue("MainCamera/KeyCodePitch_Down",mc.KeyPitch_Down);
    sett.setValue("MainCamera/KeyCodeYaw_Up",mc.KeyYaw_Up);
    sett.setValue("MainCamera/KeyCodeYaw_Down",mc.KeyYaw_Down);

    Datas::JoySetting *joyS = &(roverdata->rover.joysetting);
    sett.setValue("JOYSTICK/Throttle_Axis", joyS->throttleAxis);
    sett.setValue("JOYSTICK/Turn_Axis", joyS->turnAxis);

    sett.setValue("JOYSTICK/ARM_Actuator_1_Axis", joyS->armAct1_Axis);
    sett.setValue("JOYSTICK/ARM_Actuator_2_Axis", joyS->armAct2_Axis);
    sett.setValue("JOYSTICK/ARM_Actuator_3_Axis", joyS->armAct3_Axis);

    sett.setValue("JOYSTICK/ARM_Digger_Button_Positive_Dir", joyS->armDigger_Positive_Btn);
    sett.setValue("JOYSTICK/ARM_Digger_Button_Negetive_Dir", joyS->armDigger_Negetive_Btn);

    sett.setValue("JOYSTICK/ARM_Gripper_Actuator_1_Pos", joyS->armGripper_Act_1_Positive_Btn);
    sett.setValue("JOYSTICK/ARM_Gripper_Actuator_1_Neg", joyS->armGripper_Act_1_Negetive_Btn);

    sett.setValue("JOYSTICK/ARM_Gripper_Actuator_2_Pos", joyS->armGripper_Act_2_Positive_Btn);
    sett.setValue("JOYSTICK/ARM_Gripper_Actuator_2_Neg", joyS->armGripper_Act_2_Negetive_Btn);

    sett.setValue("JOYSTICK/Soil_Collection_Assembly_Button_Pos_Dir", joyS->soilColl_Assembly_Rotate_Povitive_Btn);
    sett.setValue("JOYSTICK/Soil_Collection_Assembly_Button_Neg_Dir", joyS->soilColl_Assembly_Rotate_Negetive_Btn);

    Datas::Xbee *x = &(roverdata->rover.xbee);
    x->baudRateIndex = settings->getBaudRateIndex();
    sett.setValue("SERIALPORT/BaudRateIndex",x->baudRateIndex);

    sett.setValue("MAP/astroPixmapSizeInPercentage",m_MapWidget->astroPixmapSizeInPercentage);

}
void MainWindow::tempFunc()
{
}

void MainWindow::handleClientConnection()
{
#if USE_TCP_SERVER
    clientConnection = tcpServer->nextPendingConnection();
#endif
}

void MainWindow::satartSoilCollPushButton_clicked()
{
    QByteArray data;
    data = encodeData(3);
    if(serial->isOpen()){
        writeData(data);
        control_ui->soilCollStatusLabel->setText("Command Sent");
    }
    else{
        terminalString.append("connect to rover to send commands");
    }

}

void MainWindow::statrBioExpPushButton_clicked()
{
    QByteArray data;
    data = encodeData(4);
    if(serial->isOpen()){
        writeData(data);
        control_ui->soilCollStatusLabel->setText("Command Sent");
    }
    else{
        terminalString.append("connect to rover to send commands");
        printTerminalString();
    }
}

void MainWindow::saveGPSPathstoFile()
{
    m_MapWidget->saveGPSPathsToFile();
    ui->statusBar->showMessage(tr("Saved gps paths to file"));
}
