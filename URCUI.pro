#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T22:54:46
#
#-------------------------------------------------

QT       += core gui printsupport xml network

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = URCUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        settingsdialog.cpp \
    joystickobject.cpp \
    joydata.cpp \
    map/ImageCache.cpp \
    map/qmapwidget.cpp \
    map/graphicsItems.cpp \
    datas.cpp \
    controller.cpp \
    datamanipulator.cpp \
    plot/urc_plot.cpp \
    roverview/urc_roverview.cpp \
    roveritem.cpp \
    map/viewgpspathdialog.cpp \
    joysticksettingdialog.cpp \
    Library/qcustomplot/qcustomplot.cpp \
    appsettingsdialog.cpp \
    encodingdialog.cpp \
    GPSMap.cpp \
    gamepad/Gamepad_private.c


win32{

    SOURCES+= gamepad/Gamepad_windows_mm.c
              #gamepad/Gamepad_windows_dinput.c \
}
linux{
    SOURCES+= gamepad/Gamepad_linux.c
}
macx{
    SOURCES+= gamepad/Gamepad_macosx.c
}

HEADERS  += mainwindow.h\
            settingsdialog.h \
    settingsdialog.h \
    joystickobject.h \
    joydata.h \
    map/ImageCache.h \
    map/qmapwidget.h \
    map/graphicsItems.h \
    datas.h \
    controller.h \
    datamanipulator.h \
    plot/urc_plot.h \
    roverview/urc_roverview.h \
    roveritem.h \
    map/viewgpspathdialog.h \
    joysticksettingdialog.h \
    Library/qcustomplot/qcustomplot.h \
    appsettingsdialog.h \
    encodingdialog.h \
    GPSMap.h \
    gamepad/Gamepad.h \
    gamepad/Gamepad_private.h

FORMS    += mainwindow.ui\
            settingsdialog.ui \
    ui/urc_controlwidget.ui \
    ui/urc_terminalwidget.ui \
    ui/urc_patheditor.ui \
    ui/urc_gpspathelement.ui \
    ui/joysticksettingdialog.ui


RESOURCES += \
    UICtrl.qrc

OTHER_FILES +=


