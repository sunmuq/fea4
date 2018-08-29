#include "squidline.h"
#include <QImage>
#include <QPainter>
#include <QDebug>

class SquidLinePrivate {
public:
	SquidLine *q;
	Point3D m_pt1,m_pt2;
	Point3D m_midpoint;
	QColor m_color;
};

SquidLine::SquidLine() 
{
	d=new SquidLinePrivate;
	d->q=this;
	d->m_pt1=point3d(0,0,0);
	d->m_pt2=point3d(0,0,0);
	d->m_midpoint=point3d(0,0,0);
}

SquidLine::~SquidLine()
{
	delete d;
}


float SquidLine::zValue(const SquidTransform &T) {
	return applyTransform(T,d->m_midpoint).z;
}
void SquidLine::render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T) {
	QList<Point3D> vertices;
	Point3D P1=applyTransform(T,d->m_pt1);
	Point3D P2=applyTransform(T,d->m_pt2);
	
	QColor col=d->m_color;
	col.setHsv(col.hue(),col.saturation(),(int)(col.value()*shadingPct()),col.alpha());
	painter.setPen(col);
	painter.drawLine(QPointF(P1.x-screen_rect.left(),P1.y-screen_rect.top()),QPointF(P2.x-screen_rect.left(),P2.y-screen_rect.top()));
}

void SquidLine::setGeometry(const Point3D &pt1,const Point3D &pt2) {
	d->m_pt1=pt1;
	d->m_pt2=pt2;
	d->m_midpoint=pt1*0.5+pt2*0.5;
}
void SquidLine::setColor(const QColor &color) {
	d->m_color=color;
}
void SquidLine::loadObject(QFile &inf,qint32 data_size) {
	QDataStream in(&inf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 version;
	in >> version;
	if (version==1) {
		read_point3d(in,d->m_pt1);
		read_point3d(in,d->m_pt2);
		read_color(in,d->m_color);
		setGeometry(d->m_pt1,d->m_pt2);
		setColor(d->m_color);
	}
	else {
		quint8 dummy;
		for (int i=0; i<data_size-4; i++) 
			in >> dummy;
	}
}
void SquidLine::saveObject(QFile &outf) {
	QDataStream out(&outf);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 version=1;
	out << version;
	write_point3d(out,d->m_pt1);
	write_point3d(out,d->m_pt2);
	write_color(out,d->m_color);
}
qint32 SquidLine::saveObjectSize() {
	return 4+6*4+4;
}
qint32 SquidLine::objectType() {
	return SQO_LINE;
}


