#ifndef squidobject_H
#define squidobject_H
#include <QPointF>
#include <QPainter>
#include <QRect>
#ifndef PI
#define PI 3.141592
#endif
#include <math.h>
#include <QFile>
#include <QVariant>

#define SQO_CUBE 1
#define SQO_LINE 2
#define SQO_IMAGE 3
#define SQO_RECT 4

struct Point3D {
	float x,y,z;
};
Point3D point3d(float x,float y,float z);
Point3D operator+(const Point3D &P1,const Point3D &P2);
Point3D operator-(const Point3D &P1,const Point3D &P2);
Point3D operator*(const Point3D &P,float val);
struct Polygon3D {
	QList<Point3D> points;
};
struct SquidTransform {
	float data[4][4];
};
void setIdentity(SquidTransform &T);
void copyTransform(SquidTransform &dst,const SquidTransform &src);
Point3D applyTransform(const SquidTransform &T,const Point3D &P);
SquidTransform multiply(const SquidTransform &T1,const SquidTransform &T2);
SquidTransform xRotation(float theta);
SquidTransform yRotation(float theta);
SquidTransform zRotation(float theta);
SquidTransform xyzScale(float xscale,float yscale,float zscale);
bool isEqual(const SquidTransform &T1,const SquidTransform &T2);
float determinant(const SquidTransform &T);

class SquidObjectPrivate;
class SquidObject {
public:
	friend class SquidObjectPrivate;
	SquidObject();
	virtual ~SquidObject();
	float shadingPct() const;
	void setShadingPct(float val);
	virtual float zValue(const SquidTransform &T)=0;
	virtual void render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T)=0;
	void setData(QString key,QVariant val);
	QVariant data(QString key) const;
	
	virtual void loadObject(QFile &inf,qint32 data_size);
	virtual void saveObject(QFile &outf);
	virtual qint32 saveObjectSize();
	virtual qint32 objectType();
	virtual bool containsPoint(const QPoint &pt,const QRectF &screen_rect,const SquidTransform &T);
private:
	SquidObjectPrivate *d;
};

void read_point3d(QDataStream &in,Point3D &pt);
void read_color(QDataStream &in,QColor &col);

void write_point3d(QDataStream &out,Point3D &pt);
void write_color(QDataStream &out,QColor &col);

#endif
