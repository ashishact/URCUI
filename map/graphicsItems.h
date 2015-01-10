#ifndef GRAPHICSITEMS_H
#define GRAPHICSITEMS_H

//#include <math.h>

#include <QPoint>
#include <QVector>
#include <QPixmap>
#include <QPainter>
#include <QColor>

#define     GI_COMPASS              1
#define     GI_MARKER               2
#define     GI_ME                   3
#define     GI_iGPS_PATH            4
#define     GI_ASTROMARKER          5
#define     GI_ARROW                6



struct GItem{
    int gitype;
    qint64 centerX, centerY;    // it is for pos wrt map, it can be very long so use qint64
    float lat,lon;
    float size;
    QString name;
    QPixmap *pix;

};

struct GPSPath{
    QVector<qint64> m_pointX;
    QVector<qint64> m_pointY;
    QVector<float>lat;
    QVector<float>lon;

    QColor col;
    QString name;
    bool active;
};


class GraphicsItems
{
public:
    GraphicsItems();
    ~GraphicsItems();
    
    void setPixmapHeading(int ang);
    void saveGPSPathsToFile();
    void draw(QPainter *p);

    QVector<GItem> item;
    QVector<GPSPath*> gpaths;

    int noOfAstro;
    int currentPathIndex;
    void addnewPath();
    void removePath(int IDx);

    void addnewPoint(int currPIDx, float lat, float lon);
    void removePoint(int PathIDx,int PointIDx);

    void setPathName(int PathIDx, QString name);
    void setPathStatus(int pathIDx, bool active);
    bool isActive(int pathIDx);
    void setPathColor(int pathIDx, QColor col);

private:
    QPixmap arrrwpix;
    QPixmap arrrpix;


    
};

#endif // GRAPHICSITEMS_H
