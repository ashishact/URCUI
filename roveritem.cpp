#include "roveritem.h"
#include <QPainter>


RoverItem::RoverItem(QPixmap *pix, int x, int y, int px, int py)
{
//    this->x = x;
//    this->y = y;
    this->item->setPos(x,y);
    this->item->setTransformOriginPoint(px,py);
    item = new QGraphicsPixmapItem(*pix);
    roll= 0;
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateroll()));
    timer.start(100);
}

RoverItem::~RoverItem()
{
    if(item){
        delete item;
    }
}

//void RoverItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//   painter->drawPixmap(QRect(x,y,pixmap->width(),pixmap->height()),*pixmap);
//}
//QRectF RoverItem::boundingRect() const
//{
//    return QRectF(0, 0,pixmap->width(),pixmap->height());
//}

void RoverItem::updateroll()
{
    static int dir = 5;
    if(roll > 350 || roll < 10)dir*=-1;
    roll = roll + dir;
    this->item->setRotation(roll);
}


