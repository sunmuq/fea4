#ifndef graphicslabelitem_H
#define graphicslabelitem_H

#include <QGraphicsItem>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>

class QGraphicsLabelItemPrivate;
class QGraphicsLabelItem : public QGraphicsItem {
public:
	friend class QGraphicsLabelItemPrivate;
	QGraphicsLabelItem(QGraphicsItem *parent=0);
	virtual ~QGraphicsLabelItem();
	void setText(QString txt);
	QString text() const;
	void setAlignment(int alignment_flags);
	void setOrientation(Qt::Orientation orientation);
	void setFont(QFont fnt);
	QRectF strictBoundingRect() const;
	virtual QRectF boundingRect() const;
	void setLinePen(QPen pen);
	void setBrush(QBrush brush);
	void setXPadding(int val);
	void setYPadding(int val);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	bool contains_view_offset(QPointF pt);
private:
	QGraphicsLabelItemPrivate *d;
};

#endif
