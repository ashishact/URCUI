#ifndef ROVERITEM_H
#define ROVERITEM_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>

class RoverItem : public QObject
{
    Q_OBJECT
public:
    RoverItem(QPixmap *pix, int x, int y, int px, int py);
    ~RoverItem();
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
//    QRectF boundingRect() const;

private slots:
    void updateroll();
public:
//    int x;
//    int y;
    int roll;
    QTimer timer;
    QGraphicsPixmapItem *item;
};




#endif // ROVERITEM_H
