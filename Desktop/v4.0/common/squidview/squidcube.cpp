#include "squidcube.h"
#include <QImage>
#include <QPainter>
#include <QDebug>

class SquidCubePrivate {
public:
	SquidCube *q;
	Point3D m_pt,m_vec1,m_vec2,m_vec3;
	Point3D m_midpoint;
	QColor m_color;
	
	QPolygonF get_polygon(const QRectF &screen_rect,const SquidTransform &T);
};

SquidCube::SquidCube() 
{
	d=new SquidCubePrivate;
	d->q=this;
	d->m_pt=point3d(0,0,0);
	d->m_vec1=point3d(1,0,0);
	d->m_vec2=point3d(0,1,0);
	d->m_vec3=point3d(0,0,1);
}

SquidCube::~SquidCube()
{
	delete d;
}

Polygon3D transform_polygon(const Polygon3D &P,const SquidTransform &T) {
	Polygon3D ret;
	for (int i=0; i<P.points.count(); i++) {
		ret.points << applyTransform(T,P.points[i]);
	}
	return ret;
}


float SquidCube::zValue(const SquidTransform &T) {
	return applyTransform(T,d->m_midpoint).z;
}

QPolygonF SquidCubePrivate::get_polygon(const QRectF &screen_rect,const SquidTransform &T) {	
	QList<Point3D> vertices;
	vertices << applyTransform(T,m_pt) << applyTransform(T,m_pt+m_vec3) << applyTransform(T,m_pt+m_vec2) << applyTransform(T,m_pt+m_vec2+m_vec3);
	vertices << applyTransform(T,m_pt+m_vec1) << applyTransform(T,m_pt+m_vec1+m_vec3) << applyTransform(T,m_pt+m_vec1+m_vec2) << applyTransform(T,m_pt+m_vec1+m_vec2+m_vec3);
	int max_ind=0;
	float max_z=vertices[0].z;
	for (int i=0; i<vertices.count(); i++) {
		if (vertices[i].z>max_z) {
			max_z=vertices[i].z;
			max_ind=i;
		}
	}
	Polygon3D P;
	//0: 111    //4: 211
	//1: 112    //5: 212
	//2: 121    //6: 221
	//3: 122    //7: 222
	if ((max_ind==0)||(max_ind==7)) {
		//111: 112,122,121,221,211,212
		P.points << vertices[1] << vertices[3] << vertices[2] << vertices[6] << vertices[4] << vertices[5];
	}
	else if ((max_ind==1)||(max_ind==6)) {
		//112: 111,121,122,222,212,211
		P.points << vertices[0] << vertices[2] << vertices[3] << vertices[7] << vertices[5] << vertices[4];
	}
	else if ((max_ind==2)||(max_ind==5)) {
		//121: 122,112,111,211,221,222,
		P.points << vertices[3] << vertices[1] << vertices[0] << vertices[4] << vertices[6] << vertices[7];
	}
	else if ((max_ind==3)||(max_ind==4)) {
		//122: 121,111,112,212,222,221,
		P.points << vertices[2] << vertices[0] << vertices[1] << vertices[5] << vertices[7] << vertices[6];
	}
	QPolygonF poly;
	for (int i=0; i<P.points.count(); i++) {
		poly << QPointF(P.points[i].x-screen_rect.left(),P.points[i].y-screen_rect.top());
	}
	return poly;
}

void SquidCube::render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T) {	
	QPolygonF poly=d->get_polygon(screen_rect,T);
	
	QColor col=d->m_color;
	col.setHsv(col.hue(),col.saturation(),(int)(col.value()*shadingPct()),col.alpha());
	painter.setBrush(QBrush(col));
	painter.setPen(Qt::NoPen);
	painter.drawPolygon(poly);
	
	/*QColor opaque_color(d->m_color.red(),d->m_color.green(),d->m_color.blue());
	float opacity=d->m_color.alpha()/255.0;
	QRectF R=poly.boundingRect();
	QRect R2((int)R.x()-2,(int)R.y()-2,(int)(R.width()+4),(int)(R.height()+4));
	QPolygonF poly2=poly;
	for (int i=0; i<poly2.count(); i++)
		poly2[i]-=R2.topLeft();
	QImage scratch_img(R2.width(),R2.height(),QImage::Format_ARGB32);
	QPainter scratch_painter(&scratch_img);
	scratch_painter.setRenderHint(QPainter::Antialiasing);
	scratch_painter.fillRect(0,0,scratch_img.width(),scratch_img.height(),Qt::black);
	scratch_painter.setBrush(QBrush(opaque_color));
	scratch_painter.setPen(QPen(opaque_color));
	scratch_painter.drawPolygon(poly2);
	for (int y=0; y<scratch_img.height(); y++)
	for (int x=0; x<scratch_img.width(); x++) {
		QColor col=QColor::fromRgb(scratch_img.pixel(x,y));
		if (col.rgb()==Qt::black) 
			col.setAlpha(0);
		else 
			col.setAlpha(d->m_color.alpha());
		scratch_img.setPixel(x,y,col.rgba());
	}
	painter.drawImage(R2,scratch_img);*/
}

void SquidCube::setGeometry(const Point3D &pt,const Point3D &vec1,const Point3D &vec2,const Point3D &vec3) {
	d->m_pt=pt;
	d->m_vec1=vec1;
	d->m_vec2=vec2;
	d->m_vec3=vec3;
	d->m_midpoint=pt+vec1*0.5+vec2*0.5+vec3*0.5;
}
void SquidCube::setColor(const QColor &color) {
	d->m_color=color;
}

void SquidCube::loadObject(QFile &inf,qint32 data_size) {
	QDataStream in(&inf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 version;
	in >> version;
	if (version==1) {
		read_point3d(in,d->m_pt);
		read_point3d(in,d->m_vec1);
		read_point3d(in,d->m_vec2);
		read_point3d(in,d->m_vec3);
		read_color(in,d->m_color);
		setGeometry(d->m_pt,d->m_vec1,d->m_vec2,d->m_vec3);
		setColor(d->m_color);
	}
	else {
		quint8 dummy;
		for (int i=0; i<data_size-4; i++) 
			in >> dummy;
	}
}
void SquidCube::saveObject(QFile &outf) {
	QDataStream out(&outf);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 version=1;
	out << version;
	write_point3d(out,d->m_pt);
	write_point3d(out,d->m_vec1);
	write_point3d(out,d->m_vec2);
	write_point3d(out,d->m_vec3);
	write_color(out,d->m_color);
}
qint32 SquidCube::saveObjectSize() {
	return 4+12*4+4;
}
qint32 SquidCube::objectType() {
	return SQO_CUBE;
}
bool SquidCube::containsPoint(const QPoint &pt,const QRectF &screen_rect,const SquidTransform &T) {
	QPolygonF P=d->get_polygon(screen_rect,T);
	return P.containsPoint(pt,Qt::OddEvenFill);
}




/////////////////////////////////////

class SquidRectPrivate {
public:
	SquidRect *q;
	Point3D m_pt,m_vec1,m_vec2;
	Point3D m_midpoint;
	QColor m_color;
	
	QPolygonF get_polygon(const QRectF &screen_rect,const SquidTransform &T);
};

SquidRect::SquidRect() 
{
	d=new SquidRectPrivate;
	d->q=this;
	d->m_pt=point3d(0,0,0);
	d->m_vec1=point3d(1,0,0);
	d->m_vec2=point3d(0,1,0);
}

SquidRect::~SquidRect()
{
	delete d;
}


float SquidRect::zValue(const SquidTransform &T) {
	return applyTransform(T,d->m_midpoint).z;
}

QPolygonF SquidRectPrivate::get_polygon(const QRectF &screen_rect,const SquidTransform &T) {	
	QList<Point3D> vertices;
	vertices << applyTransform(T,m_pt) << applyTransform(T,m_pt+m_vec1) << applyTransform(T,m_pt+m_vec1+m_vec2) << applyTransform(T,m_pt+m_vec2);
	Polygon3D P;
	P.points << vertices[0] << vertices[1] << vertices[2] << vertices[3];
	QPolygonF poly;
	for (int i=0; i<P.points.count(); i++) {
		poly << QPointF(P.points[i].x-screen_rect.left(),P.points[i].y-screen_rect.top());
	}
	return poly;
}

void SquidRect::render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T) {	
	QPolygonF poly=d->get_polygon(screen_rect,T);
	
	QColor col=d->m_color;
	col.setHsv(col.hue(),col.saturation(),(int)(col.value()*shadingPct()),col.alpha());
	painter.setBrush(QBrush(col));
	painter.setPen(Qt::NoPen);
	painter.drawPolygon(poly);
}

void SquidRect::setGeometry(const Point3D &pt,const Point3D &vec1,const Point3D &vec2) {
	d->m_pt=pt;
	d->m_vec1=vec1;
	d->m_vec2=vec2;
	d->m_midpoint=pt+vec1*0.5+vec2*0.5;
}
void SquidRect::setColor(const QColor &color) {
	d->m_color=color;
}

void SquidRect::loadObject(QFile &inf,qint32 data_size) {
	QDataStream in(&inf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 version;
	in >> version;
	if (version==1) {
		read_point3d(in,d->m_pt);
		read_point3d(in,d->m_vec1);
		read_point3d(in,d->m_vec2);
		read_color(in,d->m_color);
		setGeometry(d->m_pt,d->m_vec1,d->m_vec2);
		setColor(d->m_color);
	}
	else {
		quint8 dummy;
		for (int i=0; i<data_size-4; i++) 
			in >> dummy;
	}
}
void SquidRect::saveObject(QFile &outf) {
	QDataStream out(&outf);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 version=1;
	out << version;
	write_point3d(out,d->m_pt);
	write_point3d(out,d->m_vec1);
	write_point3d(out,d->m_vec2);
	write_color(out,d->m_color);
}
qint32 SquidRect::saveObjectSize() {
	return 4+12*3+4;
}
qint32 SquidRect::objectType() {
	return SQO_RECT;
}
bool SquidRect::containsPoint(const QPoint &pt,const QRectF &screen_rect,const SquidTransform &T) {
	QPolygonF P=d->get_polygon(screen_rect,T);
	return P.containsPoint(pt,Qt::OddEvenFill);
}




