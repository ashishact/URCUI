#ifndef VIEWGPSPATHDIALOG_H
#define VIEWGPSPATHDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include<QScrollArea>
#include<QPushButton>
#include<QCheckBox>
#include<QSignalMapper>
#include "graphicsItems.h"

class viewGPSPathDialog : public QDialog
{
    Q_OBJECT
public:
    explicit viewGPSPathDialog(GraphicsItems* gi, QWidget *parent = 0);
    void initConnections();
    void createLine(QFrame *l, bool h);
    void addARow(int i);
    void createHeader();



    QVBoxLayout *verticalLayout;
        QScrollArea *scrollArea;
        QWidget *scrollAreaWidgetContents;
        QGridLayout *gridLayout;
        QLabel *labelNameH;
        QLabel *labelSEH;
        QLabel *labelStatusH;
        QLabel *labelRemoveH;
        QLabel *labelColH;
        QLabel *noLabel;
        QLineEdit *nameLineEdit;
        QCheckBox *ActiveCheckBox;
        QLabel *colorLabel;
        QPushButton *delPushButton;
        QLabel *Start_EndLabel;
        QFrame *line;

        QHBoxLayout *horizontalLayoutBottomMost;
        QPushButton *addNewpushButton;
        QPushButton *cancelPushButton;
        QPushButton *donePushButton;
        //******************************************

        GraphicsItems* gitem;
        QSignalMapper *signalMapper;

signals:
        void signalMappingSignal(QString no);

public slots:
        void addNewPath();
        void setPathName(QString s);
        void signalMappingSlot(QString no);

};

#endif // VIEWGPSPATHDIALOG_H
