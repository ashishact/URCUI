#include "urc_roverview.h"


#include <QDebug>
#include <QPainter>
#include <QGridLayout>
#include <QGraphicsPixmapItem>
#include <QSplitter>
#include <QLinearGradient>
#include <QMenu>
#include <QPainterPath>


URC_RoverView::URC_RoverView(QWidget *parent):
    QWidget(parent)
{
    roll =0;
    //connect(&timer,SIGNAL(timeout()),this,SLOT(updateroll(int)));
    //timer.start(10);

        initialize();

        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));

        QGridLayout *layout = new QGridLayout;
//      QHBoxLayout *layout = new QHBoxLayout;

        layout->addWidget(graphicsview[0],0,0);
        layout->addWidget(graphicsview[1],0,1);
        layout->addWidget(graphicsview[2],1,0);
        layout->addWidget(graphicsview[3],1,1);

        layout->setContentsMargins(0,0,0,0);
        layout->setHorizontalSpacing(0);
        layout->setVerticalSpacing(0);
        setLayout(layout);


}


void URC_RoverView::updateroll(int roll)
{
    frontpix->setRotation(roll);
    backpix->setRotation(-roll);


    for(int i =0 ; i< 4; i++){
        graphicsview[i]->fitInView(boundary,Qt::KeepAspectRatio);
    }

}
void URC_RoverView::updatepitch(int pitch)
{
    leftpix0->setRotation(pitch);
    rightpix0->setRotation(-pitch);


    for(int i =0 ; i< 4; i++){
        graphicsview[i]->fitInView(boundary,Qt::KeepAspectRatio);
    }

}
void URC_RoverView::resizeEvent(QResizeEvent *event)
{
    for(int i =0 ; i< 4; i++){
        graphicsview[i]->fitInView(boundary,Qt::KeepAspectRatio);
    }
}
void URC_RoverView::initialize()
{
    boundary = new QGraphicsPixmapItem   (QPixmap(":/images/rover/boundary.png"));

    frontpix = new  QGraphicsPixmapItem  (QPixmap(":/images/rover/frontpix.png"));
    frontpix->setTransformOriginPoint(326,53);

    backpix = new QGraphicsPixmapItem   (QPixmap(":/images/rover/backpix.png"));
    backpix->setTransformOriginPoint(264,53);


    leftpix0 =new QGraphicsPixmapItem  (QPixmap(":/images/rover/leftpix0.png"));
    leftpix0->setPos(0,0);
    leftpix0->setTransformOriginPoint(180,26);
    leftpix1 =new QGraphicsPixmapItem  (QPixmap(":/images/rover/leftpix1.png"));
    leftpix1->setParentItem(leftpix0);
    leftpix1->setPos(260 -162,25-15);
    leftpix1->setTransformOriginPoint(162,15);
    leftpix2 =new QGraphicsPixmapItem  (QPixmap(":/images/rover/leftpix2.png"));
    leftpix2->setParentItem(leftpix1);
    leftpix2->setPos(20 -230,98-48);
    leftpix2->setTransformOriginPoint(230,48);


    rightpix0 =new QGraphicsPixmapItem ( QPixmap(":/images/rover/rightpix0.png"));
    rightpix0->setPos(0,0);
    rightpix0->setTransformOriginPoint(180,22);
    rightpix1 =new QGraphicsPixmapItem (QPixmap(":/images/rover/rightpix1.png"));
    rightpix1->setParentItem(rightpix0);
    rightpix1->setPos(130 -262,25-15);
    rightpix1->setTransformOriginPoint(262,15);
    rightpix2 =new QGraphicsPixmapItem ( QPixmap(":/images/rover/rightpix2.png"));
    rightpix2->setParentItem(rightpix1);
    rightpix2->setPos(405 -170,98-48);
    rightpix2->setTransformOriginPoint(170,48);

    for(int i =0; i< 4; i++){
        graphicsview[i] = new QGraphicsView(this);
        graphicsview[i]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsview[i]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        scene[i] = new QGraphicsScene();
        scene[i]->setSceneRect(graphicsview[i]->sceneRect());
        graphicsview[i]->setScene(scene[i]);

        QLinearGradient gradient(QPointF(0, 0), QPointF(500,500));
        gradient.setColorAt(0,QColor(255,255,212,255));
        gradient.setColorAt(1,QColor(245,201,144,255));
        QBrush b = QBrush(gradient);
//        graphicsview[i]->setBackgroundBrush(b);
//        graphicsview[i]->setStyleSheet(QStringLiteral("border-image: url(:/images/rover/roverviewbackground.png);"));
//        graphicsview[i]->setStyleSheet(QStringLiteral("border-image: url(:/images/rover/roverviewbackground0.png);"));
          graphicsview[i]->setStyleSheet(QStringLiteral("border-image: url(:/images/rover/roverviewbackground1.png);"));
//        graphicsview[i]->setStyleSheet(QStringLiteral("border-image: url(:/plot/plotbackground.png);"));


          graphicsview[i]->setRenderHint(QPainter::Antialiasing, true);
          Antialiasing = true;
          graphicsview[i]->setOptimizationFlags(QGraphicsView::DontSavePainterState);
          graphicsview[i]->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    }


    QFont timesFont("Times", 60);
    timesFont.setStyleStrategy(QFont::ForceOutline);

//        QPainterPath *textPath = new QPainterPath;

//        textPath->addText(100,-50, timesFont, tr("Front"));
//        scene[0]->addPath(*textPath);

//        textPath = new QPainterPath;
//        textPath->addText(100, -50, timesFont, tr("Back"));
//        scene[1]->addPath(*textPath);

//        textPath = new QPainterPath;
//        textPath->addText(100, -50, timesFont, tr("Left"));
//        scene[2]->addPath(*textPath);

//        textPath = new QPainterPath;
//        textPath->addText(100, -50, timesFont, tr("Right"));
//        scene[3]->addPath(*textPath);

    scene[0]->addItem(frontpix);
    scene[1]->addItem(backpix);
    scene[2]->addItem(leftpix0);
    scene[3]->addItem(rightpix0);

    QGraphicsTextItem *textit;
    textit = scene[0]->addText("Front",timesFont);
    textit->setDefaultTextColor(QColor(204,51,51));
    textit->setPos(-50,-200);

    textit = scene[1]->addText("Back",timesFont);
    textit->setDefaultTextColor(QColor(158,86,27));
    textit->setPos(-100,-200);

    textit = scene[2]->addText("Left",timesFont);
    textit->setDefaultTextColor(QColor(141,150,72));
    textit->setPos(-100,-200);

    textit = scene[3]->addText("Right",timesFont);
    textit->setDefaultTextColor(QColor(30,211,51));
    textit->setPos(-100,-200);

    for(int i =0 ; i< 4; i++){
        graphicsview[i]->fitInView(boundary,Qt::KeepAspectRatio);
    }

}

void URC_RoverView::showContextMenu(const QPoint& pos) // this is a slot
{
//    QPoint globalPos = this->mapToGlobal(pos); // no use for now
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu menu;
    QAction *toogAnti = menu.addAction("Antialiasing");
    toogAnti->setCheckable(true);
    if(Antialiasing)toogAnti->setChecked(true);
    else toogAnti->setChecked(false);

    QPoint globalPos = this->mapToGlobal(pos);
    QAction* selectedItem = menu.exec(globalPos);

    if (selectedItem)
    {
        this->toogleAntialising();
    }
    else
    {
        // nothing was chosen
    }
}
void URC_RoverView::toogleAntialising(){
    bool act = false;
    if(Antialiasing){
        act = false;
        Antialiasing = false;
        qDebug()<<"Antialising  false";
    }
    else{
        act = true;
        Antialiasing = true;
        qDebug()<<"Antialising  true";
    }

    for (int i=0; i< 4; i++){
        this->graphicsview[i]->setRenderHint(QPainter::Antialiasing,act);
    }
}
