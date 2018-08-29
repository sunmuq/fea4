#ifndef squidline_H
#define squidline_H

#include "squidobject.h"

class SquidLinePrivate;
class SquidLine : public SquidObject {
public:
	friend class SquidLinePrivate;
	SquidLine();
	virtual ~SquidLine();
	void setGeometry(const Point3D &pt1,const Point3D &pt2);
	void setColor(const QColor &color);
	
	virtual float zValue(const SquidTransform &T);
	virtual void render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T);
	
	virtual void loadObject(QFile &inf,qint32 data_size);
	virtual void saveObject(QFile &outf);
	virtual qint32 saveObjectSize();
	virtual qint32 objectType();
private:
	SquidLinePrivate *d;
};

#endif
