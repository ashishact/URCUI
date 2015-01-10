
#include <QtGui>
#include<QMenu>
#include "qmapwidget.h"

#include "ImageCache.h"
#include "graphicsItems.h"
#include "viewgpspathdialog.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QLabel>
#include <QListWidgetItem>

//ui
#include"ui_urc_patheditor.h"
#include"ui_urc_gpspathelement.h"


QMapWidget::QMapWidget()
{

    m_ViewportX = m_ViewportY = 0;
    MyGPS_spotX = MyGPS_spotY = 0;
    minZoomLevel = 1;
    maxZoomLevel = 19;
    zoomLevel=1;
    zoomlevelText = 17-zoomLevel;

    astroPixmapSizeInPercentage = 100;

    centerToSpot();
    updateLocalCordinateSystem();
    createActions();


    IsdrawcurrentPath =true;
    if(gitem.gpaths.size())gitem.currentPathIndex = 0;


}
QMapWidget::~QMapWidget()
{

}
void QMapWidget::createActions()
{
    //actionAddMarker = new QAction;
    //connect(actionAddMarker,SIGNAL(triggered()),this,addNewGraphicsItem(ME,))
}
void QMapWidget::setCache(ImageCache* imageCache){
    images = imageCache;
    if (images == NULL)
    {
        m_ViewportX = m_ViewportY = 0;
        //qDebug()<<"null";
    }
    else
    {
        m_ViewportX = 0;
        m_ViewportY = 0;
    }
}


void QMapWidget::mousePressEvent(QMouseEvent * ev)
{
    m_MouseX = ev->x();
    m_MouseY = ev->y();

}
void QMapWidget::mouseReleaseEvent(QMouseEvent *)
{

}
void QMapWidget::mouseMoveEvent(QMouseEvent * ev)
{
    m_ViewportX += m_MouseX - ev->x();
    m_ViewportY += m_MouseY - ev->y();

    m_MouseX = ev->x();
    m_MouseY = ev->y();

    updateLocalCordinateSystem();
    this->update();
}

void QMapWidget::wheelEvent(QWheelEvent *ev){

    if(ev->delta() > 0)zoomIn();
    if(ev->delta() < 0)zoomOut();

}
void QMapWidget::zoomIn(){
    setZoomLevel(zoomLevel + 1);
}

void QMapWidget::zoomOut(){
    setZoomLevel(zoomLevel - 1);
}


struct TileCoord{
    int x;
    int y;
};

QImage* QMapWidget::getTileImage(quint32 x, quint32 y, int zoom)
{
    //return image;
}

void QMapWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //painter.fillRect(0, 0, this->width(), this->height(), Qt::white);

    if (images)
    {
        // Draw tiles
        qint32 cymax = (this->m_ViewportY + this->height()) / 256;
        qint32 cxmax = (this->m_ViewportX + this->width()) / 256;
        qint32 x;
        qint32 y = -(qint32)(m_ViewportY % 256);
        qint32 cx;
        qint32 cy = this->m_ViewportY / 256;
        for (; cy <= cymax; cy++)
        {
            x = -(qint32)(m_ViewportX % 256);
            cx = this->m_ViewportX / 256;
            while (cx <= cxmax)
            {
                const QImage* pimage = images->getImage(cx,cy,zoomlevelText);
                if (pimage != NULL)
                {
                    // Draw the loaded tile
                    if (!pimage->isNull() && pimage->width() == 256 && pimage->height() == 256)
                    {
                        painter.drawImage(x, y, *pimage);
                    }
                }

                x += 256;  cx++;
            }

            y += 256;
        }

        // Draw tile bounds and numbers
        {
            char buffer[10];
            qint32 cx = this->m_ViewportX / 256;
            int x = -(int)(m_ViewportX % 256);
            int y = -(int)(m_ViewportY % 256);
            while (x < this->width())
            {
                painter.fillRect(x, 0, 1, this->height(), Qt::lightGray);

                // Draw tile X
                itoa((int)cx, buffer, 10);
                painter.drawText(x + 2, this->height() - 2, buffer);

                x += 256;  cx++;
            }
            qint32 cy = this->m_ViewportY / 256;
            while (y < this->height())
            {
                painter.fillRect(0, y, this->width(), 1, Qt::lightGray);

                // Draw tile Y
                itoa((int)cy, buffer, 10);
                painter.drawText(2, y + 12, buffer);

                y += 256;  cy++;
            }
        }

    }


   drawPathLegend(&painter);
   gitem.draw(&painter);


    images->update(); // Deleting Loaded Images from MAP
}
void QMapWidget::drawPathLegend(QPainter *p)
{
    p->setBrush(Qt::lightGray);
    int loc_noOfPath = gitem.gpaths.size();
    int loc_x = this->width() - 80;
    int loc_y = 2;
    int loc_w = 78;
    int loc_h = 10 + loc_noOfPath * 10;
    p->drawRect(loc_x,loc_y,loc_w,loc_h);

    for(int pathno = 0; pathno <gitem.gpaths.size(); pathno++){
        QPen pen;  // creates a default pen
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        pen.setColor(gitem.gpaths.at(pathno)->col);

        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        p->setPen(pen);
        int legendx = loc_x + 5;
        int legendy = loc_y + pathno * 10 + 12;

        p->drawLine(QPoint(legendx,legendy-4),QPoint(legendx+20 ,legendy-4));
        p->drawText(legendx + 30 , legendy , gitem.gpaths.at(pathno)->name);
    }
}

void QMapWidget::convertLatLonToXY(double lat, double lon, qint64 *px, qint64 *py)
{
    double xf = (lon + 180.0) / 360.0 * pow(2.0, zoomLevel);
    double yf = (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, zoomLevel);
    *px = (qint64)floor(xf * 256);
    *py = (qint64)floor(yf * 256);
}

void QMapWidget::centerToSpot()
{
    if (gitem.item.at(0).centerX == 0 && gitem.item.at(0).centerY == 0)
        return;  // There is no spot
    this->centerTo(MyGPS_spotX,MyGPS_spotY);

}
void QMapWidget::centerTo(qint64 x, qint64 y)
{
    m_ViewportX = x - this->width() / 2;
    m_ViewportY = y - this->height() / 2;

    updateLocalCordinateSystem();
    this->update();
}

void QMapWidget::setMyGPSPosition(double lat, double lon)
{
    qint64 spotxnew, spotynew;
    this->convertLatLonToXY(lat, lon, &spotxnew, &spotynew);
    if (MyGPS_spotX != spotxnew || MyGPS_spotY != spotynew)
    {
        MyGPS_spotX = spotxnew;
        MyGPS_spotY = spotynew;
//        this->update();
    }
}

void QMapWidget::setGItemPosWrtMapWidget(bool gpspath, int idx, double lat, double lon)
{
    if(gpspath){
        if(lat || lon){
            qint64 newx, newy;
            this->convertLatLonToXY(gitem.gpaths.at(idx)->lat.last(),
                                    gitem.gpaths.at(idx)->lon.last(),
                                    &newx, &newy);

            newx = newx - m_ViewportX;
            newy = newy - m_ViewportY;

            gitem.gpaths.at(idx)->m_pointX[gitem.gpaths.at(idx)->lat.size()-1] = newx;
            gitem.gpaths.at(idx)->m_pointY[gitem.gpaths.at(idx)->lat.size()-1] = newy;
        }
        else{

            for(int i=0; i<gitem.gpaths.at(idx)->lat.size(); i++){
                qint64 newx, newy;
                this->convertLatLonToXY(gitem.gpaths.at(idx)->lat.at(i),
                                        gitem.gpaths.at(idx)->lon.at(i),
                                        &newx, &newy);

                newx = newx - m_ViewportX;
                newy = newy - m_ViewportY;

                gitem.gpaths.at(idx)->m_pointX[i] = newx;
                gitem.gpaths.at(idx)->m_pointY[i] = newy;
            }
        }
    }
    else{
        qint64 newx, newy;
        this->convertLatLonToXY(lat, lon, &newx, &newy);

        newx = newx - m_ViewportX;
        newy = newy - m_ViewportY;

        if(idx < gitem.item.size()){    // if item exists in the vector
            gitem.item[idx].centerX = newx;
            gitem.item[idx].centerY = newy;
        }
    }

}
void QMapWidget::updateLocalCordinateSystem()
{
    if(gitem.item.size()>0){
        setGItemPosWrtMapWidget(false,0,gitem.item.at(0).lat,gitem.item.at(0).lon);
        setMyGPSPosition(gitem.item.at(0).lat,gitem.item.at(0).lon);   //we this need to center the map to gps location
    }
    for(int i =2; i< gitem.item.size(); i++){
        if(gitem.item.at(i).gitype != GI_COMPASS){
            setGItemPosWrtMapWidget(false,i ,gitem.item[i].lat,gitem.item[i].lon);
        }
    }
    for(int i=0; i<gitem.gpaths.size();i++){
        setGItemPosWrtMapWidget(true,i,0,0);
    }
}
inline int remapToPow2(int num, int oldPow, int newPow){
    return (oldPow > newPow) ?
        num >> (oldPow - newPow):
        num << (newPow - oldPow);
}
inline int fitToPow2(int num, int power){
    int val = 1 << power;
    int mask = val - 1;
    if (num < 0)
        num = val - ((-num)&mask);
    return num & mask;
}

void QMapWidget::setZoomLevel(int level){
    if ((level >= minZoomLevel) && (level <= maxZoomLevel)){
        if (zoomLevel != level){
            adjustCenter();
            int x_Center = m_ViewportX + this->width() / 2;
            int y_Center = m_ViewportY + this->height() / 2;
            m_ViewportX = remapToPow2(x_Center, zoomLevel, level) - this->width() / 2;
            m_ViewportY = remapToPow2(y_Center, zoomLevel, level) - this->height() / 2;
            zoomLevel = level;
            zoomlevelText = 17-zoomLevel;
            emit zoomLevelChanged(zoomLevel);
            repaint();
        }
        //emit canZoomIn(level < 16);
        //emit canZoomOut(level > 0);
    }
    updateLocalCordinateSystem();
    this->update();
}



void QMapWidget::adjustCenter(){
    int TilePower2=8;
    int xCenter = m_ViewportX + this->width() / 2;
    int yCenter = m_ViewportY + this->height() / 2;

    xCenter = fitToPow2(xCenter, zoomLevel+TilePower2);
    yCenter = fitToPow2(yCenter, zoomLevel+TilePower2);

    m_ViewportX= xCenter - this->width() / 2;
    m_ViewportY= yCenter - this->height() / 2;

}




void QMapWidget::addAstroMarker()
{

    QDialog dialog(this);                                       // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);                                  // Add some text above the fields

    form.addRow(new QLabel("Enter Astronaut's Name & location"));      // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;

    QLineEdit *lineedit = new QLineEdit(&dialog);
    lineedit->setText("Ashish Act");
    QString labelName("Name");
    form.addRow(labelName, lineedit);
    fields << lineedit;


    lineedit = new QLineEdit(&dialog);
    lineedit->setValidator(new QDoubleValidator(-180,180,4,lineedit));
    lineedit->setText("19.1336");
    QString labellat("Latitude");
    form.addRow(labellat, lineedit);
    fields << lineedit;


    lineedit = new QLineEdit(&dialog);
    lineedit->setValidator(new QDoubleValidator(-90,90,4,lineedit));
    lineedit->setText("72.9154");
    QString labellon("Longitude");
    form.addRow(labellon, lineedit);
    fields << lineedit;


        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        //      qDebug() << lineEdit->text();

        GItem *gi = new GItem();
        gi->gitype = GI_ASTROMARKER;
        gi->name = "Astronaut";
        int pixNo= gitem.noOfAstro%5;
        gi->pix = new QPixmap(":/map/images/map/astro"+QString::number(pixNo)+".png");
        *(gi->pix) = gi->pix->scaled(astroPixmapSizeInPercentage,
                                     astroPixmapSizeInPercentage,
                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
        gi->centerX = 0;
        gi->centerY =0;
        gi->lat =0;
        gi->lon =0;
        gi->size =1.0;

        gitem.item.append(*gi); gitem.noOfAstro++;
        gitem.item[gitem.item.size()-1].name = fields.at(0)->text();
        gitem.item[gitem.item.size()-1].lat = fields.at(1)->text().toDouble();
        gitem.item[gitem.item.size()-1].lon = fields.at(2)->text().toDouble();


    }
    updateLocalCordinateSystem();
    this->update();
}

void QMapWidget::addNewGPSPath()
{
    bool ok;
    QString pathName = QInputDialog::getText(this, tr("GPS Path Name"),
                                         tr("name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok){
        QColor color = QColorDialog::getColor(Qt::black, this, "GPS Path Color",  QColorDialog::DontUseNativeDialog);
         if(!color.isValid())
         {
           color = QColor(12, 200, 123);
         }

         gitem.addnewPath();
         int index = gitem.gpaths.size()-1;
         gitem.currentPathIndex = index;

         gitem.setPathColor(index,color);
         if (!pathName.isEmpty())
             gitem.setPathName(index,pathName);
         gitem.setPathStatus(index,true);
    }

    IsdrawcurrentPath = true;

}

void QMapWidget::deleteGPSPath(QAction * act)
{
    int index = act->data().toInt();
    if(index > 2000){
        if(index == 3000)IsdrawcurrentPath = false;
        else if(index == 3001)IsdrawcurrentPath =true;
    }
    else{
        gitem.removePath(index);
        gitem.currentPathIndex= gitem.gpaths.size()-1;
        IsdrawcurrentPath = false;
    }
this->update();
}
void QMapWidget::deleteAstroItem(QAction * act)
{
    int index = act->data().toInt();
    gitem.item.remove(index);
    this->update();
}


void QMapWidget::editGPSPathList(){
    viewGPSPathDialog *vgpg = new viewGPSPathDialog(&gitem,this);
    vgpg->show();
}

void QMapWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *centerToThis= new QAction("Center To This",this);
    centerToThis->setStatusTip("center the map to any Latitude & Longitude");
    connect(centerToThis,SIGNAL(triggered()),this,SLOT(centerToThis()));
    menu.addAction(centerToThis);

    QAction *editGPSPathAction= new QAction("Edit Paths",this);
    editGPSPathAction->setStatusTip("edit it");
    connect(editGPSPathAction,SIGNAL(triggered()),this,SLOT(editGPSPathList()));
    menu.addAction(editGPSPathAction);

    QAction *addAstroMarkerAction= new QAction("New Astronaut",this);
    addAstroMarkerAction->setStatusTip("Adds a placemarker with for an Astraunaut");
    connect(addAstroMarkerAction,SIGNAL(triggered()),this,SLOT(addAstroMarker()));

    menu.addAction(addAstroMarkerAction);

    QVector<QAction*> astroItemActionList;
    QMenu *astroItemSubMenu = menu.addMenu("Astronuts");
    for(int i =0; i < gitem.item.size(); i++){
        if(gitem.item.at(i).gitype == GI_ASTROMARKER){
            astroItemActionList.append(astroItemSubMenu->addAction(
                                           QIcon(*(gitem.item.at(i).pix)),
                                           "Remove "+gitem.item.at(i).name+" :"+ QString::number(i-2)));
            astroItemActionList.last()->setData(i);
        }
    }
    connect(astroItemSubMenu,SIGNAL(triggered(QAction*)),this,SLOT(deleteAstroItem(QAction*)));


    QAction *addNewGPSPathAction = new QAction(QIcon(":/map/images/map/path.ico"),"add a new Path",this);
    addNewGPSPathAction->setStatusTip("Add a new path to record for the Rover");
    connect(addNewGPSPathAction,SIGNAL(triggered()),this,SLOT(addNewGPSPath()));




    QVector<QAction*> gpsPathActionList;
    QMenu *gpsPathSubMenu = menu.addMenu("GPS Path");
    for(int i =0; i < gitem.gpaths.size(); i++){
        gpsPathActionList.append(gpsPathSubMenu->addAction("Remove "+gitem.gpaths.at(i)->name+" :"+ QString::number(i)));
        gpsPathActionList.at(i)->setData(i);

    }
    gpsPathActionList.append(gpsPathSubMenu->addAction("Draw Current Path"));
    gpsPathActionList.at(gpsPathActionList.size()-1)->setData(3001);
    gpsPathActionList.append(gpsPathSubMenu->addAction("Stop Drawing Path"));
    gpsPathActionList.at(gpsPathActionList.size()-1)->setData(3000);


    connect(gpsPathSubMenu,SIGNAL(triggered(QAction*)),this,SLOT(deleteGPSPath(QAction*)));



    menu.addAction(addNewGPSPathAction);

    QPoint globalPos = this->mapToGlobal(event->pos());
    menu.exec(globalPos);
}

void QMapWidget::setPixmapHeading(int ang){
    this->gitem.setPixmapHeading(ang);
    this->update();
}
void QMapWidget::updateGPSValues(float lat, float lon)
{
    float minLatLonDiff = 0.0001;
    bool Laddnewpoint = false;
    float latdiff = lat - gitem.item[0].lat;
    float londiff = lon - gitem.item[0].lon;
    if(latdiff > minLatLonDiff || latdiff < -minLatLonDiff
            || londiff > minLatLonDiff || londiff < -minLatLonDiff){
        Laddnewpoint = true;
    }
    gitem.item[0].lon = lon;
    gitem.item[0].lat = lat;
    setGItemPosWrtMapWidget(false,0,lat,lon);
    if(IsdrawcurrentPath && Laddnewpoint){
        gitem.addnewPoint(gitem.currentPathIndex,lat,lon);
    }
    updateLocalCordinateSystem();
}

void QMapWidget::saveGPSPathsToFile()
{
  gitem.saveGPSPathsToFile();
}
void QMapWidget::centerToThis(){

    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("Enter Lat & Lon"));
    QList<QLineEdit *> fields;


    QLineEdit *lineedit = new QLineEdit(&dialog);
    lineedit->setValidator(new QDoubleValidator(-180,180,4,lineedit));
    lineedit->setText("19.1336");
    QString labellat("Latitude");
    form.addRow(labellat, lineedit);
    fields << lineedit;


    lineedit = new QLineEdit(&dialog);
    lineedit->setValidator(new QDoubleValidator(-90,90,4,lineedit));
    lineedit->setText("72.9154");
    QString labellon("Longitude");
    form.addRow(labellon, lineedit);
    fields << lineedit;

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        double lat = fields.at(0)->text().toDouble();
        double lon = fields.at(1)->text().toDouble();
        if(!lat)lat = 19.1336;
        if(!lon)lon = 72.9154;

        qint64 xnew, ynew;
        this->convertLatLonToXY(lat, lon, &xnew, &ynew);

        this->centerTo(xnew,ynew);
        this->update();
    }
}
