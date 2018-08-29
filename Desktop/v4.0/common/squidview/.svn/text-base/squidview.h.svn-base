#ifndef squidview_H
#define squidview_H

#include <QWidget>
#include "squidviewbase.h"
#include <QPoint>

class SquidView : public QWidget, public SquidViewBase {
	Q_OBJECT
public:
	SquidView(QWidget *parent=0);
	virtual ~SquidView();
signals:
	void transformed();
	void objectClicked(SquidObject *obj);
protected:
	void updateScreen();
	int screenWidth();
	int screenHeight();
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
private:
	bool m_processing_move_event;
	QPoint m_last_mouse_pos;
	bool m_move_since_last_press;
};



#endif
