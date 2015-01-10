
#include "ImageCache.h"
#include <QFile>
#include <QColor>
#include <QPainter>


ImageCache::~ImageCache(){
}
void ImageCache::setCacheDir(const QString& newCacheDir){
	if (cacheDir != newCacheDir){
		cacheDir = newCacheDir;
		if (cacheDir.size())
			if (cacheDir[cacheDir.size()-1] != QChar('/'))
				cacheDir.append("/");
		clear();
		emit imageChanged();
	}
}

const QString& ImageCache::getCacheDir(){
	return cacheDir;
}

ImageCache::ImageCache()
    :filenametemplate("%1/%2/%3/%4/%5.png"),cacheDir("tiles/"){
}

QImage* ImageCache::getImage(int x, int y, int zoom){
	TileCoord tileCoord(x, y, zoom);
	
    if (!images.contains(tileCoord))
        loadImage(tileCoord);

    if (images.contains(tileCoord)){
        Tile& tile = images[tileCoord];
        return &tile.image;
    }
	return 0;
}

ImageCache::TileCoord::TileCoord()
:x(0), y(0), zoom(0){
}

ImageCache::TileCoord::TileCoord(const int _x, const int _y, const int _zoom)
:x(_x), y(_y), zoom(_zoom){
}

ImageCache::TileCoord::TileCoord(const TileCoord& tc)
:x(tc.x), y(tc.y), zoom(tc.zoom){
}

bool operator< (const ImageCache::TileCoord& arg1, const ImageCache::TileCoord arg2){
	if (arg1.zoom < arg2.zoom) return true;
	if (arg1.y < arg2.y) return true;
	if (arg1.x < arg2.x) return true;
	return false;
}

ImageCache::Tile::Tile()
:life(DefaultLife){
}

ImageCache::Tile::Tile(const QImage& _image)
:image(_image), life(DefaultLife){
}
void ImageCache::loadImage(const TileCoord& tileCoord){
    QString filename = getCacheName(tileCoord.x, tileCoord.y, tileCoord.zoom);
    //printf(filename.toLatin1().data());printf("\n");
    if (QFile::exists(filename)){
		images[tileCoord] = Tile(QImage());
		if (!images[tileCoord].image.load(filename)){
//				QFile::remove(filename);//should work with broken files
			images.remove(tileCoord);
		}
	}
}


QString ImageCache::getCacheName(int x, int y, int zoom){
	return QString("%1%2").arg(cacheDir)
		.arg(makeFileName(x, y, zoom));
}

QString ImageCache::makeFileName(int x, int y, int zoom){
    int xBase = x/1024;
    int yBase = y/1024;
    return QString(filenametemplate)
        .replace("%1", QString::number(zoom))
        .replace("%2", QString::number(xBase))
        .replace("%3", QString::number(x%1024))
        .replace("%4", QString::number(yBase))
        .replace("%5", QString::number(y%1024));
}

void ImageCache::update(){
	for (QMap<TileCoord, Tile>::iterator i = images.begin(); i != images.end();){
		QMap<TileCoord, Tile>::iterator cur = i; i++;
		cur->countdown();
		if (cur->getLife() <= 0)
			images.erase(cur);
	}
}

void ImageCache::clear(){
	images.clear();
}

QString ImageCache::getCoordsQstr(int x, int y, int zoom){
	static const char table[2][2] = {
		{'q', 'r'},
		{'t', 's'}
	};
	QString tmp("");
	for (;zoom > 0; zoom--){
		tmp.prepend(table[y&1][x&1]);
		x = x>>1;
		y = y>>1;
	}
	return tmp;
}
