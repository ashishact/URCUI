
#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#define followTarget 0

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QVector>
#include<QAction>

#include "ImageCache.h"
#include "graphicsItems.h"



class QMapWidget : public QWidget
{
    Q_OBJECT
public:
    QMapWidget();
    ~QMapWidget();

    void createActions();
    ImageCache *images; // change to private later

public:
    void setGItemPosWrtMapWidget(bool gpspath, int idx, double lat, double lon);
    void updateLocalCordinateSystem();
    void setMyGPSPosition(double lat, double lon);
    void drawPathLegend(QPainter *p);
#if followTarget
    void setTargetPosition(double lat, double lon);
    void resetTargetPosition();
#endif
    void centerTo(qint64 x, qint64 y);
    void centerToSpot();

    void setPixmapHeading(int ang);


    QAction *actionAddMarker;

    int astroPixmapSizeInPercentage;


public slots:
    void zoomIn();
    void zoomOut();
    void setZoomLevel(int level);

    void setCache(ImageCache* cache);
    //int getZoomLevel();
    //QPoint getCoords();
    void addAstroMarker();
    void addNewGPSPath();
    void deleteGPSPath(QAction*act);
    void deleteAstroItem(QAction*act);
    void editGPSPathList();
    void updateGPSValues(float lat, float lon);
    void saveGPSPathsToFile();
    void centerToThis();

signals:
    void canZoomIn(bool status);
    void canZoomOut(bool status);
    void zoomLevelChanged(int level);

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void wheelEvent(QWheelEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void paintEvent(QPaintEvent *);
    void adjustCenter();

protected slots:
#if followTarget
    void targetTimerTick();

#endif
private:
    QImage* getTileImage(quint32 x, quint32 y, int zoom);
    void convertLatLonToXY(double lat, double lon, qint64* px, qint64* py);

private:
    int zoomLevel;
    int zoomlevelText;
    int minZoomLevel;
    int maxZoomLevel;
    qint64 m_ViewportX, m_ViewportY;
    int m_MouseX, m_MouseY;
    qint64 MyGPS_spotX, MyGPS_spotY;
    GraphicsItems gitem;

    bool IsdrawcurrentPath;


};

#endif // QMAPWIDGET_H
