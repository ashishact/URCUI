#include "viewgpspathdialog.h"

#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include<QScrollArea>
#include<QPushButton>
#include<QCheckBox>
#include<QAction>

#include<QToolButton>
#include<QDebug>

viewGPSPathDialog::viewGPSPathDialog(GraphicsItems *gi, QWidget *parent) :
    QDialog(parent)
{
    this->gitem = gi;

    signalMapper = new QSignalMapper(this);

    this->setWindowTitle("Edit Gps Path");
       verticalLayout = new QVBoxLayout(this);


       scrollArea = new QScrollArea(this);
       scrollArea->setWidgetResizable(true);
       scrollAreaWidgetContents = new QWidget();

       gridLayout = new QGridLayout(scrollAreaWidgetContents);
       // 0th row
       createHeader();

       for(int i =0; i < gitem->gpaths.size(); i++){
       // one row
            addARow(i);

       // row finished
       }
       connect(signalMapper, SIGNAL(mapped(QString)),
                   this, SIGNAL(signalMappingSignal(QString)));
       connect(this,SIGNAL(signalMappingSignal(QString)),this,SLOT(signalMappingSlot(QString)));

          scrollArea->setWidget(scrollAreaWidgetContents);
          verticalLayout->addWidget(scrollArea);
    // bottommost options
          horizontalLayoutBottomMost = new QHBoxLayout();


          cancelPushButton = new QPushButton(this);
          cancelPushButton->setText("Cancel");
          horizontalLayoutBottomMost->addWidget(cancelPushButton);

          addNewpushButton = new QPushButton(this);
          addNewpushButton->setText("Add New");
//          connect(addNewpushButton,SIGNAL(clicked()),this,SLOT(addNewPath()));
          horizontalLayoutBottomMost->addWidget(addNewpushButton);

          donePushButton = new QPushButton(this);
          donePushButton->setText("Done");
          connect(donePushButton,SIGNAL(clicked()),this,SLOT(accept()));
          horizontalLayoutBottomMost->addWidget(donePushButton);

          verticalLayout->addLayout(horizontalLayoutBottomMost);

}
void viewGPSPathDialog::addARow(int i)
{
    noLabel = new QLabel(scrollAreaWidgetContents);
    noLabel->setText(QString::number(i));
    gridLayout->addWidget(noLabel, i+2, 0, 1, 1);


    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,i+2,1,1,1);

    nameLineEdit = new QLineEdit(scrollAreaWidgetContents);
    nameLineEdit->setObjectName(QString::number(i));
    nameLineEdit->setText(gitem->gpaths.at(i)->name);
    connect(nameLineEdit, SIGNAL(returnPressed()), signalMapper, SLOT(map()));
    signalMapper->setMapping(nameLineEdit, nameLineEdit->objectName());
    gridLayout->addWidget(nameLineEdit, i+2, 2, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,i+2,3,1,1);

    Start_EndLabel = new QLabel(scrollAreaWidgetContents);
    QString lasf, losf, lasl, losl;
    if(gitem->gpaths.at(i)->lat.size()){
        lasf = QString::number(gitem->gpaths.at(i)->lat.front());
        losf = QString::number(gitem->gpaths.at(i)->lon.front());

        lasl = QString::number(gitem->gpaths.at(i)->lat.last());
        losl = QString::number(gitem->gpaths.at(i)->lon.last());
    }
    else{
        lasf = "null";
        losf = "null";
        lasl = "null";
        losl = "null";
    }
    Start_EndLabel->setText("("+lasf+","+losf+"0) -> ("+lasl+","+losl+")");
    gridLayout->addWidget(Start_EndLabel, i+2, 4, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,i+2,5,1,1);

    ActiveCheckBox = new QCheckBox(scrollAreaWidgetContents);
    ActiveCheckBox->setChecked(gitem->gpaths.at(i)->active);
    ActiveCheckBox->setObjectName(QString::number(gitem->gpaths.size()+i));
    connect(ActiveCheckBox, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ActiveCheckBox, ActiveCheckBox->objectName());
    gridLayout->addWidget(ActiveCheckBox, i+2,6, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,i+2,7,1,1);

    colorLabel = new QLabel(scrollAreaWidgetContents);
    colorLabel->setText("COLOR");
    QColor c = gitem->gpaths.at(i)->col;
    QPalette palette = colorLabel->palette();
    palette.setColor(colorLabel->foregroundRole(),c);
     colorLabel->setPalette(palette);
    gridLayout->addWidget(colorLabel, i+2, 8, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,i+2,9,1,1);

    delPushButton = new QPushButton(scrollAreaWidgetContents);
    delPushButton->setObjectName(QString::number(2*gitem->gpaths.size()+i));
    delPushButton->setText("Remove");
    connect(delPushButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(delPushButton, delPushButton->objectName());

    gridLayout->addWidget(delPushButton, i+2, 10, 1, 1);

}
void viewGPSPathDialog::createHeader()
{
    labelNameH = new QLabel(scrollAreaWidgetContents);
    labelNameH->setText("No");
    labelNameH->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelNameH, 0, 0, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,0,1,1,1);

    labelNameH = new QLabel(scrollAreaWidgetContents);
    labelNameH->setText("Name");
    labelNameH->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelNameH, 0, 2, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,0,3,1,1);

    labelSEH = new QLabel(scrollAreaWidgetContents);
    labelSEH->setText("Start -> End");
    labelSEH->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelSEH, 0, 4, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,0,5,1,1);

    labelStatusH = new QLabel(scrollAreaWidgetContents);
    labelStatusH->setText("Status");
    labelStatusH->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelStatusH, 0, 6, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,0,7,1,1);

    labelColH = new QLabel(scrollAreaWidgetContents);
    labelColH->setText("Color");
    labelColH->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelColH, 0, 8, 1, 1);

    line = new QFrame;
    createLine(line,false);
    gridLayout->addWidget(line,0,9,1,1);

    labelRemoveH = new QLabel(scrollAreaWidgetContents);
    labelRemoveH->setText("Remove");
    labelRemoveH->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelRemoveH, 0, 10, 1, 1);

    for (int i =0; i < 11; i++){
         line = new QFrame;
         createLine(line,true);
         gridLayout->addWidget(line,1,i,1,1);
    }
}
void viewGPSPathDialog::initConnections()
{

}
void viewGPSPathDialog::createLine(QFrame *l, bool h)
{
    if(h){
        l->setFrameShape(QFrame::HLine);
    }
    else{
        l->setFrameShape(QFrame::VLine);
    }
    l->setFrameShadow(QFrame::Sunken);
}

void viewGPSPathDialog::setPathName(QString s){

}
void viewGPSPathDialog::signalMappingSlot(QString no){
    if(no.toInt() < gitem->gpaths.size()){//name
        int pathIdx = no.toInt();
    }
    else if(no.toInt() < 2*gitem->gpaths.size()){//check
        int pathIdx = no.toInt() - gitem->gpaths.size();
        if(gitem->isActive(pathIdx)){
            gitem->setPathStatus(pathIdx ,false);
        }
        else{
            gitem->setPathStatus(pathIdx ,true);
        }
    }
    else if(no.toInt() < 3*gitem->gpaths.size()){//delete
        int pathIdx = no.toInt() - 2*gitem->gpaths.size();
        gitem->removePath(pathIdx);
    }
}
void viewGPSPathDialog::addNewPath()
{
    gitem->addnewPath();
    addARow(gitem->gpaths.size()-1);
}
