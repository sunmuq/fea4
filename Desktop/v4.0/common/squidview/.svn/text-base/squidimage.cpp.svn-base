#include "squidimage.h"
#include <QPainter>
#include <QDebug>

class SquidImagePrivate {
public:
	SquidImage *q;
	Point3D m_pt;
	Point3D m_vec1,m_vec2;
	Point3D m_midpoint;
	QImage m_image;
};

SquidImage::SquidImage() 
{
	d=new SquidImagePrivate;
	d->q=this;
}

SquidImage::~SquidImage()
{
	delete d;
}

void SquidImage::setGeometry(const Point3D &pt,const Point3D &vec1,const Point3D &vec2) {
	d->m_pt=pt;
	d->m_vec1=vec1;
	d->m_vec2=vec2;
	d->m_midpoint=pt+vec1*0.5+vec2*0.5;
}
void SquidImage::setImage(const QImage &X) {
	d->m_image=X;
}
float SquidImage::zValue(const SquidTransform &T) {
	return applyTransform(T,d->m_midpoint).z;
}
void draw_image(QPainter &painter,const QImage &img,QPointF pt,QPointF vec1,QPointF vec2,float shading_pct) {
	QImage img2=img;
	for (int y=0; y<img2.height(); y++)
	for (int x=0; x<img2.width(); x++) {
		QColor col=QColor::fromRgba(img2.pixel(x,y));
		col.setHsv(col.hue(),col.saturation(),(int)(col.value()*shading_pct),col.alpha());
		img2.setPixel(x,y,col.rgba());
	}
	QTransform hold=painter.transform();
	QTransform T;
	//(0,0) -> (pt.x,pt.y)
	//(1,0) -> (pt.x+vec1.x,pt.y+vec1.y)
	//(0,1) -> (pt.x+vec2.x,pt.y+vec2.y)
	T.setMatrix(vec1.x(),vec1.y(),0,vec2.x(),vec2.y(),0,pt.x(),pt.y(),1);
	painter.setTransform(T);
	painter.drawImage(QRectF(0,0,1,1),img2,QRect(0,0,img.width(),img.height()));
	painter.setTransform(hold);
}
void SquidImage::render(QPainter &painter,const QRectF &screen_rect,const SquidTransform &T) {
	Point3D pt=applyTransform(T,d->m_pt);
	Point3D vec1=applyTransform(T,d->m_pt+d->m_vec1)-pt;
	Point3D vec2=applyTransform(T,d->m_pt+d->m_vec2)-pt;
	pt.x-=screen_rect.left();
	pt.y-=screen_rect.top();
	draw_image(painter,d->m_image,QPointF(pt.x,pt.y),QPointF(vec1.x,vec1.y),QPointF(vec2.x,vec2.y),shadingPct());
}

void SquidImage::loadObject(QFile &inf,qint32 data_size) {
	QDataStream in(&inf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 version;
	in >> version;
	if (version==1) {
		read_point3d(in,d->m_pt);
		read_point3d(in,d->m_vec1);
		read_point3d(in,d->m_vec2);
		setGeometry(d->m_pt,d->m_vec1,d->m_vec2);
		qint32 w;
		qint32 h;
		in >> w >> h;
		QImage img(w,h,QImage::Format_ARGB32);
		for (int y=0; y<h; y++)
		for (int x=0; x<w; x++) {
			QColor col;
			read_color(in,col);
			img.setPixel(x,y,col.rgba());
		}
		setImage(img);
	}
	else {
		quint8 dummy;
		for (int i=0; i<data_size-4; i++) 
			in >> dummy;
	}
}
void SquidImage::saveObject(QFile &outf) {
	QDataStream out(&outf);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 version=1;
	out << version;
	write_point3d(out,d->m_pt);
	write_point3d(out,d->m_vec1);
	write_point3d(out,d->m_vec2);
	qint32 w=d->m_image.width();
	qint32 h=d->m_image.height();
	out << w << h;
	for (int y=0; y<h; y++)
	for (int x=0; x<w; x++) {
		QColor col=QColor::fromRgba(d->m_image.pixel(x,y));
		write_color(out,col);
	}
}
qint32 SquidImage::saveObjectSize() {
	return 4+12*3+4+4+d->m_image.width()*d->m_image.height()*4;
}
qint32 SquidImage::objectType() {
	return SQO_IMAGE;
}


