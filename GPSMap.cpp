

#include <QtWidgets>

#include "GPSMap.h"

//! [0]
GPSMap::GPSMap(QWidget *parent)
    : QScrollArea(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    drawPath=false;
    myPenWidth = 1;
    myPenColor = Qt::green;

    lastPoint = QPoint(1,0);
}

bool GPSMap::openMap(const QString &fileName)
{
    QImage loadedMap;
    if (!loadedMap.load(fileName))
        return false;

    //QSize newSize = loadedMap.size().expandedTo(size());
    //resizeImage(&loadedMap, newSize);
    map = loadedMap;
    modified = false;
    update();
    return true;
}

/*bool GPSMap::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = map;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
*/
void GPSMap::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void GPSMap::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}
void GPSMap::clearMap()
{
    //modified = true;
    update();
}

void GPSMap::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawPath=true;
    }
    if (event->button() == Qt::RightButton) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            this->openMap(fileName);
    }
}

/*void GPSMap::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void GPSMap::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}
*/

void GPSMap::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, map, dirtyRect);
}

/*void GPSMap::resizeEvent(QResizeEvent *event)
{
    if (width() > map.width() || height() > map.height()) {
        int newWidth = qMax(width() + 128, map.width());
        int newHeight = qMax(height() + 128, map.height());
        resizeImage(&map, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
*/

void GPSMap::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&map);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}
