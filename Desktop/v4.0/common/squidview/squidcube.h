#ifndef squidcube_H
#define squidcube_H

#include "squidobject.h"

class SquidCubePrivate;
class SquidCube : public SquidObject {
public:
	friend class SquidCubePrivate;
	SquidCube();
	virtual ~SquidCube();
	void setGeometry(const Point3D &pt,const Point3D &vec1,const Point3D &vec2,const Point3D &vec3);
	void setColor(const QColor &color);
	
	virtual float zValue(const SquidTransform &T);
	virtual void render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T);
	
	virtual void loadObject(QFile &inf,qint32 data_size);
	virtual void saveObject(QFile &outf);
	virtual qint32 saveObjectSize();
	virtual qint32 objectType();
	virtual bool containsPoint(const QPoint &pt,const QRectF &screen_rect,const SquidTransform &T);
private:
	SquidCubePrivate *d;
};

class SquidRectPrivate;
class SquidRect : public SquidObject {
public:
	friend class SquidRectPrivate;
	SquidRect();
	virtual ~SquidRect();
	void setGeometry(const Point3D &pt,const Point3D &vec1,const Point3D &vec2);
	void setColor(const QColor &color);
	
	virtual float zValue(const SquidTransform &T);
	virtual void render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T);
	
	virtual void loadObject(QFile &inf,qint32 data_size);
	virtual void saveObject(QFile &outf);
	virtual qint32 saveObjectSize();
	virtual qint32 objectType();
	virtual bool containsPoint(const QPoint &pt,const QRectF &screen_rect,const SquidTransform &T);
private:
	SquidRectPrivate *d;
};

#endif
