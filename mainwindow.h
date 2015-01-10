#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QVector>

#include "joystickobject.h"
#include "map/qmapwidget.h"
#include "datas.h"
#include "joysticksettingdialog.h"

#include <plot/urc_plot.h>
#include <roverview/urc_roverview.h>


namespace Ui {
class MainWindow;
class URC_ControlWidget;
class URC_TerminalWidget;
}
class joystickObject;
class SettingsDialog;
class JoystickSettingDialog;
class ImageCache;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void printTerminalString();
    void initqmapwidget();
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void tempFunc();

public slots:
    void joyEvent(unsigned int deviceID, bool btnEvent, unsigned int axisNo);
    void joystickFound(int n);
    void toggleTerminal();
    // GPS
        void centerMapToSpot();
        void handleClientConnection();
#if followTarget
        void followSpot();
#endif
        //void initkeycommand();
protected:

public slots:
    void timerEvent();
    void openSerialPort(); // Default in out serial port
    void closeSerialPort();

    //void about();
    void writeData(const QByteArray &data);
    void readData();
    QByteArray encodeData(const int type);  //  0 for rover & 1 for arm
    void decodeData();

    void updateGPSValues(double lat,double lon, int region);
    void updateIMUValues(int roll, int pitch, int heading);

    void saveGPSPathstoFile();

    void handleError(QSerialPort::SerialPortError error);
    void resetAll();

    void setupAdditionalUis();
    void loadSettingFile();
    void saveToSettingFile();
private slots:
    void stopRover();
    void stopArm();
    void changeCacheDirButton_clicked();

    void satartSoilCollPushButton_clicked();

    void statrBioExpPushButton_clicked();

private:
    void initActionsConnections();  // connect all actions here
    void initRest();
    void createDockWindows();
QListWidget *paragraphsList;QListWidget *customerList;
private:
    Ui::MainWindow *ui;
    Ui::URC_ControlWidget *control_ui; QTabWidget *control_ui_tw;
    Ui::URC_TerminalWidget *terminal_ui;QWidget *terminal_ui_w;

    URC_Plot *urc_plot;
    URC_RoverView *urc_roverview;

    QTimer m_timer;
    QSlider *mapzoomsslider;
    QVector<QString> terminalString;//bool terminalActive;
    SettingsDialog *settings;
    JoystickSettingDialog *joysticksettingdialog;
    QSerialPort *serial;
    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;
    joystickObject m_joy;
    Datas *roverdata;

    QByteArray comportdata;
    //QByteArray GPScomportdata;
    //char current_identifier;
    int maxcomportdatasize;


    //GPS Map
    ImageCache imageCache;

    QMapWidget* m_MapWidget;

    //Settings file
    QString m_settingsFile;

signals:
    void decodecomportdatasig();
};

#endif // MAINWINDOW_H
