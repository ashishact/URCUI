#ifndef GPSMAP_H
#define GPSMAP_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QScrollArea>

class GPSMap : public QScrollArea
{
    Q_OBJECT

public:
    GPSMap(QWidget *parent = 0);
    void init();
    bool openMap(const QString &fileName);
    void clearMap();
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    //void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    //void resizeEvent(QResizeEvent *event);

public:
    void drawLineTo(const QPoint &endPoint);

    bool modified;
    int myPenWidth;
    int currentMap;
    bool drawPath;

    QColor myPenColor;
    QImage map;
    QPoint lastPoint;

    QPoint RoverGPSPos;
};

#endif // GPSMAP_H
