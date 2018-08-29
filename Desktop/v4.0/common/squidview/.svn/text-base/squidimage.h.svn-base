#ifndef squidimage_H
#define squidimage_H

#include "squidobject.h"

class SquidImagePrivate;
class SquidImage : public SquidObject {
public:
	friend class SquidImagePrivate;
	SquidImage();
	virtual ~SquidImage();
	void setGeometry(const Point3D &pt,const Point3D &vec1,const Point3D &vec2);
	void setImage(const QImage &X);
	
	virtual float zValue(const SquidTransform &T);
	virtual void render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T);
	
	virtual void loadObject(QFile &inf,qint32 data_size);
	virtual void saveObject(QFile &outf);
	virtual qint32 saveObjectSize();
	virtual qint32 objectType();
private:
	SquidImagePrivate *d;
};

#endif
