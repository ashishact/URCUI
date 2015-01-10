#ifndef JOYSTICKSETTINGDIALOG_H
#define JOYSTICKSETTINGDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>

#include "datas.h"
#include "joydata.h"


QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class JoystickSettingDialog;
}

QT_END_NAMESPACE


class JoystickSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JoystickSettingDialog(QWidget *parent, Datas::JoySetting *joysettings, JoyData *j);
    ~JoystickSettingDialog();
public slots:
    void joyEvent(unsigned int deviceID, bool btnEvent, unsigned int axisNo);
    void joystickFound(int n);
private slots:
    void apply();
private:
    void updateSettings();
    void additionalui();
    void fillDatas();
    void initConnections();
private:
    Ui::JoystickSettingDialog *ui;
    Datas::JoySetting *joyS;
    Datas::JoySetting tempjoyS;
    JoyData *joydata;


    QHBoxLayout *horizontalLayout[12];
    QLabel *label[12];
    QComboBox *combobox[12];
    QProgressBar *progressbar[12];


};

#endif // JOYSTICKSETTINGDIALOG_H
