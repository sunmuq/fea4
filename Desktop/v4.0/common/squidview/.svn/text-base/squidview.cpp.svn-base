#include "squidview.h"
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>

SquidView::SquidView(QWidget *parent) : QWidget(parent) {
	m_processing_move_event=false;
	m_move_since_last_press=false;
	setFocusPolicy(Qt::StrongFocus);
}
SquidView::~SquidView() {
}

void SquidView::updateScreen() {
	update();
}
int SquidView::screenWidth() {
	return width();
}
int SquidView::screenHeight() {
	return height();
}

void SquidView::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawImage(0,0,image());
	QWidget::paintEvent(event);	
}
void SquidView::resizeEvent(QResizeEvent *event) {
	refresh();
}
void SquidView::mouseMoveEvent(QMouseEvent *event) {
	m_move_since_last_press=true;
	if (m_processing_move_event) return;
	m_processing_move_event=true;
	if (event->buttons()&Qt::LeftButton) {
		QPoint diff=event->globalPos()-m_last_mouse_pos;
		double theta1=-diff.x()*1.0/2;
		double theta2=-diff.y()*1.0/2;
		SquidTransform T=transform();
		SquidTransform T1=yRotation(theta1);
		SquidTransform T2=xRotation(theta2);
		T=multiply(T1,T);
		T=multiply(T2,T);
		setTransform(T);
		emit transformed();
		refresh();
		qApp->processEvents();
	}
	else if (event->buttons()&Qt::RightButton) {
		QPoint diff=event->globalPos()-m_last_mouse_pos;
		double theta=0;
		if (event->pos().x()>width()/2) theta+=diff.y()/2;
		else theta-=diff.y()/2;
		if (event->pos().y()>height()/2) theta-=diff.x()/2;
		else theta+=diff.x()/2;
		SquidTransform T=transform();
		SquidTransform T1=zRotation(theta);
		T=multiply(T1,T);
		setTransform(T);
		emit transformed();
		refresh();
		qApp->processEvents();
	}
	else QWidget::mouseMoveEvent(event);
	m_last_mouse_pos=event->globalPos();
	m_processing_move_event=false;
	updateScreen();
}
void SquidView::mousePressEvent(QMouseEvent *event) {
	m_move_since_last_press=false;
	m_last_mouse_pos=event->globalPos();
	QWidget::mousePressEvent(event);
}
void SquidView::keyPressEvent(QKeyEvent *event) {
	if (event->modifiers()&&Qt::ControlModifier) {
		if (event->key()==61) { //Ctrl+'='
			float factor=1.4;
			SquidTransform T=transform();
			T=multiply(xyzScale(factor,factor,factor),T);
			setTransform(T);
			emit transformed();
			refresh();
		}
		else if (event->key()==45) { //Ctrl+'-'
			float factor=1/1.4;
			SquidTransform T=transform();
			T=multiply(xyzScale(factor,factor,factor),T);
			setTransform(T);
			emit transformed();
			refresh();
		}
	}
}
void SquidView::mouseReleaseEvent(QMouseEvent *event) {
	if (!m_move_since_last_press) {
		SquidObject *obj=objectAtPoint(event->pos());
		if (obj) emit objectClicked(obj);
	}
}
