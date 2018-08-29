#ifndef squidviewbase_H
#define squidviewbase_H
#include <QImage>

#include "squidobject.h"
#include <QThread>

class SquidViewPrivate;
class SquidViewBaseHelper;
class SquidViewBase {
public:
	friend class SquidViewPrivate;
	friend class SquidViewBaseHelper;
	SquidViewBase();
	virtual ~SquidViewBase();
	int objectCount() const;
	SquidObject *object(int index) const;
	void addObject(SquidObject *obj);
	void clearObjects();
	void setTransform(const SquidTransform &T);
	SquidTransform transform() const;
	void setBackgroundColor(QColor col);
	
	bool loadView(QString fname);
	bool saveView(QString fname);
	
	QImage image() const;
	void refresh(bool run_in_separate_thread=true);
	SquidObject *objectAtPoint(QPoint pt) const; //pt in the image
protected:	
	virtual void updateScreen()=0;
	virtual int screenWidth()=0;
	virtual int screenHeight()=0;
private:
	SquidViewPrivate *d;
};

class SquidViewThreadPrivate;
class SquidViewThread : public QThread {
	Q_OBJECT
public:
	friend class SquidViewThreadPrivate;
	SquidViewThread();
	virtual ~SquidViewThread();
	int objectCount() const;
	SquidObject *object(int index) const;
	void addObject(SquidObject *obj);
	void clearObjects();
	void setTransform(const SquidTransform &T);
	SquidTransform transform() const;
	void setScreenDimensions(int w,int h);
	void setScreenRect(const QRectF &R);
	void setBackgroundColor(QColor col);
	QRectF screenRect() const;
	QImage image() const;
	void haltThread();
	void lockThread();
	void unlockThread();
	void run();
signals:
	void updated();
private:
	SquidViewThreadPrivate *d;
};

class SquidViewBaseHelper : public QObject {
	Q_OBJECT
public:
	SquidViewBase *SVI;
public slots:
	void slot_updated();
};


#endif
