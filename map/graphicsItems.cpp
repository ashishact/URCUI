#include "graphicsItems.h"
#include <QDebug>
#include <QTransform>
#include<QFile>
#include<qmath.h>

#define DRAWCIRCULARSPOT 0

GraphicsItems::GraphicsItems()
{



    GItem *gi = new GItem;
    gi->gitype = GI_ME;
    gi->centerX = 0;
    gi->centerY = 0;
    gi->name = "GPS";
    gi->lat = 19.1336;
    gi->lon = 72.9154;
    gi->pix = new QPixmap(":/map/images/map/arrowred.png");
    *(gi->pix) = gi->pix->scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    gi->size = 1.0;
    arrrpix = *(gi->pix);

    item.append(*gi);

  // compass
    gi = new GItem;
    gi->gitype = GI_COMPASS;
    gi->name = "COMPASS";
    gi->lat = 0;
    gi->lon = 0;
    gi->pix = new QPixmap(":/map/images/map/compass.png");
    *(gi->pix) = gi->pix->scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    int offs=5;
    gi->centerX =0 + offs ;
    gi->centerY =0 + offs ;
    gi->size = 0.7;
    item.append(*gi);

    int comBaseW = gi->pix->width();
    int comBaseH = gi->pix->height();


    // compass arrow
      gi = new GItem;
      gi->gitype = GI_COMPASS;
      gi->name = "COM_ARROW";
      gi->lat = 0;
      gi->lon = 0;
      gi->pix = new QPixmap(":/map/images/map/arrowredwhite.png");
      *(gi->pix) = gi->pix->scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);


      //int arrowBaseW = gi->pix->width();
      //int arrowBaseH = gi->pix->height();

      gi->centerX =0 + offs + comBaseW/2;// - arrowBaseW/2 ;
      gi->centerY =0 + offs + comBaseH/2;// - arrowBaseH/2 ;
      gi->size = 2;

      arrrwpix = *(gi->pix);

      item.append(*gi);

      // astro
      noOfAstro = 0;
      // path gps
     addnewPath();


}
GraphicsItems::~GraphicsItems()
{
    //delete &imagepixmap;
}


void GraphicsItems::draw(QPainter *p)
{
    for(int i=0; i<item.size();i++){

       qint64 posX = item[i].centerX;
       qint64 posY = item[i].centerY;
       if(item[i].gitype == GI_ME){
           const qint64 spothalfsize = 20;
           const qint64 spotarchalfsize = 14;
           qint64 sx=posX;
           qint64 sy=posY;
           int r=6;    // r is radius of the circle
           p->setRenderHint(QPainter::Antialiasing,true);
           p->setPen(QPen(Qt::green, 3));
#if DRAWCIRCULARSPOT
               p->fillRect(sx - spothalfsize, sy, spothalfsize*2, 1, Qt::red);
               p->fillRect(sx, sy - spothalfsize, 1, spothalfsize*2, Qt::red);
               p->setPen(Qt::red);
               p->drawArc(sx - spotarchalfsize, sy - spotarchalfsize, spotarchalfsize*2, spotarchalfsize*2, 0, 5760);
#endif
               if(!arrrpix.isNull()){
                    int w = arrrpix.width();
                    int h = arrrpix.height();
                    p->drawPixmap(item[i].centerX - w/2  , item[i].centerY -h/2, w, h ,arrrpix);
               }

            QRadialGradient gradient(0, 0, r*0.8);
               gradient.setCenter(sx,sy);
               gradient.setFocalPoint(sx,sy);
               gradient.setColorAt(1, QColor(Qt::red));
               gradient.setColorAt(0, QColor(Qt::red).light(30));

            p->setBrush(gradient);

            p->setPen(QPen(Qt::black, 0));
            p->drawEllipse(sx-r,sy-r, 2*r,2*r);


       }

       else if(item[i].gitype== GI_ASTROMARKER){
           if(item[i].pix){
                int w = item[i].pix->width();
                int h = item[i].pix->height();
                p->drawPixmap(item[i].centerX - w/2 , item[i].centerY - h ,*(item[i].pix));

           }
       }

       else if(item[i].gitype== GI_COMPASS){
           if(item[i].pix){
               if(item[i].size == 2){                   
                   int w = arrrwpix.width();
                   int h = arrrwpix.height();
                   p->drawPixmap(item[i].centerX -w/2  , item[i].centerY -h/2, w, h ,arrrwpix);
               }
               else{
                   int w = item[i].pix->width();
                   int h = item[i].pix->height();
                   p->drawPixmap(item[i].centerX  , item[i].centerY, w, h ,*(item[i].pix));
               }


           }
       }


       else if(item[i].gitype== GI_iGPS_PATH){
           if(item[i].pix){
               QPixmap pix;
               if(item[i].size == 2){
                   QTransform transform;
                   QTransform trans = transform.rotate(-30);
                   pix = item[i].pix->transformed(trans,Qt::SmoothTransformation);
               }
               else{
                   pix = *(item[i].pix);
               }

                int w = pix.width();
                int h = pix.height();
                p->drawPixmap(item[i].centerX  , item[i].centerY, w, h ,pix);

           }
       }


   }    // for loop finished

    // GPS paths

   for(int pid=0; pid< gpaths.size(); pid++){
        if(!gpaths.at(pid)->active)continue;
       QPen pen;  // creates a default pen
       pen.setStyle(Qt::SolidLine);
       pen.setWidth(2);
       pen.setColor(gpaths.at(pid)->col);
       pen.setCapStyle(Qt::RoundCap);
       pen.setJoinStyle(Qt::RoundJoin);
       p->setPen(pen);

       for(int i =0; i<gpaths.at(pid)->lat.size()-1; i++){
               p->drawLine(gpaths.at(pid)->m_pointX.at(i),gpaths.at(pid)->m_pointY.at(i),
                           gpaths.at(pid)->m_pointX.at(i+1),gpaths.at(pid)->m_pointY.at(i+1));
       }
   }


}
void GraphicsItems::addnewPath(){

    GPSPath *gp = new  GPSPath;
//    gp->lat.append(19.1336);
//    gp->lon.append(72.9154);
//    gp->m_pointX.append(0);
//    gp->m_pointY.append(0);

//    gp->lat.append(18.1336);
//    gp->lon.append(72.9154);
//    gp->m_pointX.append(0);
//    gp->m_pointY.append(0);

//    gp->lat.append(19.1336);
//    gp->lon.append(71.9154);
//    gp->m_pointX.append(0);
//    gp->m_pointY.append(0);

    gp->col = QColor(0,250,0);
    gp->active = true;
    gp->name = "Path: " + QString::number(gpaths.size()+1);

    gpaths.append(gp);
 }
void GraphicsItems::removePath(int IDx){
    if(gpaths.size() >IDx){
        gpaths.remove(IDx);
    }
    if(gpaths.size()){
        currentPathIndex = gpaths.size()-1;
    }
}

void GraphicsItems::addnewPoint(int currPIDx, float lat, float lon){
    if(gpaths.size() && gpaths.size() > currPIDx){
        gpaths.at(currPIDx)->lat.append(lat);
        gpaths.at(currPIDx)->lon.append(lon);
        gpaths.at(currPIDx)->m_pointX.append(0);
        gpaths.at(currPIDx)->m_pointY.append(0);
    }
}
void GraphicsItems::removePoint(int PathIDx,int PointIDx){
    if(gpaths.size()>PathIDx){
        if(gpaths.at(PathIDx)->lat.size()>PointIDx){
            gpaths.at(PathIDx)->lat.remove(PointIDx);
            gpaths.at(PathIDx)->lon.remove(PointIDx);
            gpaths.at(PathIDx)->m_pointX.remove(PointIDx);
            gpaths.at(PathIDx)->m_pointY.remove(PointIDx);
        }
    }
}
void GraphicsItems::saveGPSPathsToFile()
{
    QFile file("Gps_paths.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    QTextStream out(&file);

    for(int pid=0; pid< gpaths.size(); pid++){
        out << "GPS Path No: " << pid+1 <<"\n\n";
        for(int i =0; i<gpaths.at(pid)->lat.size(); i++){


                   out << gpaths.at(pid)->lat.at(i);
                   out << ",";
                   out << gpaths.at(pid)->lon.at(i);
                   out << "\n";
        }
    }
    file.close();
}

void GraphicsItems::setPathName(int PathIDx, QString name){
    if(gpaths.size()>PathIDx){
        gpaths[PathIDx]->name = name;
    }
}
void GraphicsItems::setPathStatus(int pathIDx, bool active){
    if(gpaths.size()>pathIDx){
        gpaths[pathIDx]->active = active;
    }
}

bool GraphicsItems::isActive(int pathIDx){
    if(gpaths.size()>pathIDx){
        return gpaths[pathIDx]->active;
    }
    else return false;
}
void GraphicsItems::setPathColor(int pathIDx, QColor col){
    if(gpaths.size()>pathIDx){
        gpaths[pathIDx]->col = col;
    }
}

void GraphicsItems::setPixmapHeading(int ang){
    if(item.size() >= 3){

        QTransform transform;
        QTransform trans = transform.rotate(ang);
        arrrpix = item[0].pix->transformed(trans,Qt::SmoothTransformation);
        QTransform transform2;
        trans = transform2.rotate(ang-30);
        arrrwpix = item[2].pix->transformed(trans,Qt::SmoothTransformation);

    }
}
