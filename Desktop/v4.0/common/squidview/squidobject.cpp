#include "squidobject.h"

class SquidObjectPrivate {
public:
	SquidObject *q;
	bool m_show_for_negative_z;
	float m_shading_pct;
	QHash<QString,QVariant> m_data;
};

SquidObject::SquidObject() 
{
	d=new SquidObjectPrivate;
	d->q=this;
	d->m_show_for_negative_z=true;
}

SquidObject::~SquidObject()
{
	delete d;
}

Point3D point3d(float x,float y,float z) {
	Point3D ret;
	ret.x=x;
	ret.y=y;
	ret.z=z;
	return ret;
}
Point3D operator+(const Point3D &P1,const Point3D &P2) {
	Point3D ret;
	ret.x=P1.x+P2.x;
	ret.y=P1.y+P2.y;
	ret.z=P1.z+P2.z;
	return ret;
}
Point3D operator-(const Point3D &P1,const Point3D &P2) {
	Point3D ret;
	ret.x=P1.x-P2.x;
	ret.y=P1.y-P2.y;
	ret.z=P1.z-P2.z;
	return ret;
}
Point3D operator*(const Point3D &P,float val) {
	Point3D ret;
	ret.x=P.x*val;
	ret.y=P.y*val;
	ret.z=P.z*val;
	return ret;
}
Point3D applyTransform(const SquidTransform &T,const Point3D &P) {
	Point3D ret;
	ret.x=T.data[0][0]*P.x+T.data[0][1]*P.y+T.data[0][2]*P.z+T.data[0][3];
	ret.y=T.data[1][0]*P.x+T.data[1][1]*P.y+T.data[1][2]*P.z+T.data[1][3];
	ret.z=T.data[2][0]*P.x+T.data[2][1]*P.y+T.data[2][2]*P.z+T.data[2][3];
	return ret;
}
void setIdentity(SquidTransform &T) {
	for (int i=0; i<4; i++)
	for (int j=0; j<4; j++) {
		if (i==j) T.data[i][j]=1;
		else T.data[i][j]=0;
	}
}
void copyTransform(SquidTransform &dst,const SquidTransform &src) {
	for (int i=0; i<4; i++)
	for (int j=0; j<4; j++) {
		dst.data[i][j]=src.data[i][j];
	}
}
SquidTransform multiply(const SquidTransform &T1,const SquidTransform &T2) {
	SquidTransform ret;
	for (int i=0; i<4; i++)
	for (int j=0; j<4; j++) {
		float val=0;
		for (int k=0; k<4; k++) val+=T1.data[i][k]*T2.data[k][j];
		ret.data[i][j]=val;
	}
	return ret;
}
SquidTransform xRotation(float theta) {
	SquidTransform T;
	setIdentity(T);
	float ang=theta*PI/180;
	T.data[1][1]=cos(ang);
	T.data[2][2]=cos(ang);
	T.data[1][2]=-sin(ang);
	T.data[2][1]=sin(ang);	
	return T;
}
SquidTransform yRotation(float theta) {
	SquidTransform T;
	setIdentity(T);
	float ang=theta*PI/180;
	T.data[0][0]=cos(ang);
	T.data[2][2]=cos(ang);
	T.data[0][2]=-sin(ang);
	T.data[2][0]=sin(ang);	
	return T;
}
SquidTransform zRotation(float theta) {
	SquidTransform T;
	setIdentity(T);
	float ang=theta*PI/180;
	T.data[0][0]=cos(ang);
	T.data[1][1]=cos(ang);
	T.data[0][1]=-sin(ang);
	T.data[1][0]=sin(ang);	
	return T;
}
SquidTransform xyzScale(float xscale,float yscale,float zscale) {
	SquidTransform T;
	setIdentity(T);
	T.data[0][0]=xscale;
	T.data[1][1]=yscale;
	T.data[2][2]=zscale;
	return T;
}
float determinant(const SquidTransform &T) {
	float ret=0;
	ret+=T.data[0][0]*T.data[1][1]*T.data[2][2];
	ret+=T.data[1][0]*T.data[2][1]*T.data[0][2];
	ret+=T.data[2][0]*T.data[0][1]*T.data[1][2];
	ret-=T.data[0][2]*T.data[1][1]*T.data[2][0];
	ret-=T.data[1][2]*T.data[2][1]*T.data[0][0];
	ret-=T.data[2][2]*T.data[0][1]*T.data[1][0];
	return ret;
}
bool isEqual(const SquidTransform &T1,const SquidTransform &T2) {
	for (int i=0; i<4; i++)
	for (int j=0; j<4; j++) 
		if (T1.data[i][j]!=T2.data[i][j]) return false;
	return true;
}
float SquidObject::shadingPct() const {
	return d->m_shading_pct;
}
void SquidObject::setShadingPct(float val) {
	d->m_shading_pct=val;
}

void SquidObject::loadObject(QFile &inf,qint32 data_size) {
}
void SquidObject::saveObject(QFile &outf) {
}
qint32 SquidObject::saveObjectSize() {
	return 0;
}
qint32 SquidObject::objectType() {
	return 0;
}
bool SquidObject::containsPoint(const QPoint &pt,const QRectF &screen_rect,const SquidTransform &T) {
	return false;
}
void read_point3d(QDataStream &in,Point3D &pt) {
	float x,y,z;
	in >> x >> y >> z;
	pt.x=x; 
	pt.y=y;
	pt.z=z;
}
int put_in_range(int x,int xmin,int xmax) {
	if (x<xmin) return xmin;
	if (x>xmax) return xmax;
	return x;
}
void read_color(QDataStream &in,QColor &col) {
	quint8 a,b,c,d;
	in >> a >> b >> c >> d;
	a=put_in_range(a,0,255);
	b=put_in_range(b,0,255);
	c=put_in_range(c,0,255);
	d=put_in_range(d,0,255);
	col=QColor(a,b,c,d);
}
void write_point3d(QDataStream &out,Point3D &pt) {
	float x,y,z;
	x=pt.x;
	y=pt.y;
	z=pt.z;
	out << x << y << z;
}
void write_color(QDataStream &out,QColor &col) {
	quint8 a,b,c,d;
	a=col.red();
	b=col.green();
	c=col.blue();
	d=col.alpha();
	out << a << b << c << d;
}

void SquidObject::setData(QString key,QVariant val) {
	d->m_data[key]=val;
}
QVariant SquidObject::data(QString key) const {
	return d->m_data.value(key);
}

