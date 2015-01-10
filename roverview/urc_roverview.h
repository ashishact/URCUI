#ifndef URC_ROVERVIEW_H
#define URC_ROVERVIEW_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QSplitter>
#include <QDockWidget>
#include <QPixmap>

#include <QTimer>


class URC_RoverView : public QWidget
{
    Q_OBJECT
public:
    explicit URC_RoverView(QWidget *parent =0);
    void resizeEvent(QResizeEvent *event);
    void initialize();
public slots:
    void updateroll(int roll);
    void updatepitch(int pitch);
private slots:

    void showContextMenu(const QPoint& pos);
    void toogleAntialising();
public:

QGraphicsPixmapItem *boundary;

    QGraphicsPixmapItem *frontpix;
    QGraphicsPixmapItem *backpix;

    QGraphicsPixmapItem *leftpix;
    QGraphicsPixmapItem *leftpix0;
    QGraphicsPixmapItem *leftpix1;
    QGraphicsPixmapItem *leftpix2;

    QGraphicsPixmapItem *rightpix;
    QGraphicsPixmapItem *rightpix0;
    QGraphicsPixmapItem *rightpix1;
    QGraphicsPixmapItem *rightpix2;


    QGraphicsScene *scene[4];

    QGraphicsView *graphicsview[4];



private:
    QTimer timer;
    int roll;
    bool Antialiasing;

};



#endif // URC_ROVERVIEW_H
