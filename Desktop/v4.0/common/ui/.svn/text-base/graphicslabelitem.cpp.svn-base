#include "graphicslabelitem.h"
#include <QDebug>

class QGraphicsLabelItemPrivate {
public:
	QGraphicsLabelItem *q;
	QFont m_font;
	QString m_text;
	int m_alignment_flags;
	Qt::Orientation m_orientation;
	QPen m_line_pen;
	QBrush m_brush;
	int m_x_padding;
	int m_y_padding;
};

QGraphicsLabelItem::QGraphicsLabelItem(QGraphicsItem *parent) : QGraphicsItem(parent) 
{
	d=new QGraphicsLabelItemPrivate;
	d->q=this;
	d->m_line_pen.setStyle(Qt::NoPen);
	d->m_alignment_flags=0;
	d->m_orientation=Qt::Horizontal;
	d->m_x_padding=2;
	d->m_y_padding=2;
	setFlags(QGraphicsItem::ItemIgnoresTransformations);
}

QGraphicsLabelItem::~QGraphicsLabelItem()
{
	delete d;
}
void QGraphicsLabelItem::setText(QString txt) {
	update(boundingRect());	
	d->m_text=txt;
	update(boundingRect());
};
QString QGraphicsLabelItem::text() const {
	return d->m_text;
}
void QGraphicsLabelItem::setAlignment(int alignment_flags) {
	if (d->m_alignment_flags!=alignment_flags) {
		d->m_alignment_flags=alignment_flags;
		prepareGeometryChange();
	}
}
void QGraphicsLabelItem::setOrientation(Qt::Orientation orientation) {
	if (d->m_orientation!=orientation) {
		d->m_orientation=orientation;
		prepareGeometryChange();
	}
}
void QGraphicsLabelItem::setFont(QFont fnt) {
	if (d->m_font!=fnt) {			
		d->m_font=fnt;
		prepareGeometryChange();
	}
}

QRectF QGraphicsLabelItem::strictBoundingRect() const {
	QFontMetrics fm(d->m_font);
	QStringList lines=d->m_text.split("\n");
	int num_lines=lines.count();
	int wid=0;
	for (int i=0; i<num_lines; i++)
		wid=qMax(wid,fm.width(lines[i]));
	QRectF rect;
	if (d->m_orientation==Qt::Horizontal)
		rect=QRectF(0,0,wid,fm.height()*num_lines);
	else
		rect=QRectF(0,0,fm.height()*num_lines,wid);
	
	if (d->m_alignment_flags&Qt::AlignRight)
		rect.moveTo(-rect.width(),rect.y());
	else if (d->m_alignment_flags&Qt::AlignHCenter)
		rect.moveTo(-rect.width()/2,rect.y());
		
	if (d->m_alignment_flags&Qt::AlignBottom)
		rect.moveTo(rect.x(),-rect.height());
	else if (d->m_alignment_flags&Qt::AlignVCenter)
		rect.moveTo(rect.x(),-rect.height()/2);
	
	return rect;
}
QRectF QGraphicsLabelItem::boundingRect() const {
	QRectF ret=strictBoundingRect();	
	return QRectF(ret.x()-d->m_x_padding,ret.y()-d->m_y_padding,ret.width()+d->m_x_padding*2,ret.height()+d->m_y_padding*2);
}
bool QGraphicsLabelItem::contains_view_offset(QPointF pt) {
	QRectF BR=boundingRect();
	QPointF pt2=pt-BR.topLeft();
	return ((0<=pt2.x())&&(pt2.x()<BR.width())&&(0<=pt2.y())&&(pt2.y()<BR.height()));
}
void QGraphicsLabelItem::setLinePen(QPen pen) {
	d->m_line_pen=pen;
}
void QGraphicsLabelItem::setBrush(QBrush brush) {
	d->m_brush=brush;
}
void QGraphicsLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	if (d->m_brush.style()!=Qt::NoBrush) {
		painter->fillRect(boundingRect(),d->m_brush);
	}
	if (d->m_line_pen.style()!=Qt::NoPen) {
		QPen old_pen=painter->pen();
		painter->setPen(d->m_line_pen);
		painter->drawRect(boundingRect());
		painter->setPen(old_pen);
	}
	
	painter->setFont(d->m_font);
	if (isSelected()) {
		QPen selected_pen(Qt::DashLine);
		selected_pen.setColor(Qt::black);
		painter->setPen(selected_pen);
		painter->drawRect(strictBoundingRect());
	}
	if (d->m_orientation==Qt::Horizontal)
		painter->drawText(strictBoundingRect(),d->m_text);
	else {
		QRectF br=strictBoundingRect();
		float x1=br.x(); 
		//float x2=br.x()+br.width();
		//float y1=br.y(); 
		float y2=br.y()+br.height();
		
		painter->save();
		painter->translate(x1,y2);
		painter->rotate(-90);
		painter->drawText(QRectF(0,0,br.height(),br.width()),d->m_text);
		painter->restore();
	}
	
	//painter->drawRoundRect(strictBoundingRect());
}
void QGraphicsLabelItem::setXPadding(int val) {
	d->m_x_padding=val;
}
void QGraphicsLabelItem::setYPadding(int val) {
	d->m_y_padding=val;
}

