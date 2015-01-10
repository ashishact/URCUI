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
    gamepad/Gamepad_windows.c \
    gamepad/Gamepad_private.c \
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
    Library/qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h\
            settingsdialog.h \
    settingsdialog.h \
    gamepad/Gamepad_private.h \
    gamepad/Gamepad.h \
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
    Library/qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui\
            settingsdialog.ui \
    ui/urc_controlwidget.ui \
    ui/urc_terminalwidget.ui \
    ui/urc_patheditor.ui \
    ui/urc_gpspathelement.ui \
    ui/joysticksettingdialog.ui

LIBS += -gamepad/lstem_gamepad32.a -lwinmm

RESOURCES += \
    UICtrl.qrc

OTHER_FILES +=


