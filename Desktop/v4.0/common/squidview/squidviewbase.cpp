#include "squidviewbase.h"

#include <QMutex>
#include <QMutexLocker>
#include <QTime>
#include "squidarray2d.h"
#include <QDebug>
#include <QFile>
#include "squidcube.h"
#include "squidline.h"
#include "squidimage.h"
#include "rtfmriutils.h"

#define SQV_OBJECTS 1
#define SQV_TRANSFORM 2

SquidObject *create_object_by_type(qint32 otype) {
	if (otype==SQO_CUBE) return new SquidCube;
	else if (otype==SQO_LINE) return new SquidLine;
	else if (otype==SQO_IMAGE) return new SquidImage;
	else if (otype==SQO_RECT) return new SquidRect;
	else return 0;
}



class SquidViewThreadPrivate {
public:
	SquidViewThread *q;
	QList<SquidObject *> m_objects;
	SquidTransform m_transform;
	QImage m_image;
	QSize m_screen_size;
	QRectF m_screen_rect;
	bool m_need_to_rerender;
	bool m_halt_thread;
	QMutex m_mutex;
	QColor m_background_color;
};


SquidViewThread::SquidViewThread() {
	d=new SquidViewThreadPrivate;
	d->q=this;
	d->m_halt_thread=false;
	d->m_background_color=Qt::black;
}

SquidViewThread::~SquidViewThread() {
	clearObjects();
	delete d;
}
int SquidViewThread::objectCount() const {
	QMutexLocker locker(&d->m_mutex);
	return d->m_objects.count();
}
SquidObject *SquidViewThread::object(int index) const {
	QMutexLocker locker(&d->m_mutex);
	return d->m_objects[index];
}
void SquidViewThread::addObject(SquidObject *obj) {
	QMutexLocker locker(&d->m_mutex);
	d->m_objects << obj;
}
void SquidViewThread::clearObjects() {
	QMutexLocker locker(&d->m_mutex);
	qDeleteAll(d->m_objects);
	d->m_objects.clear();
}
void SquidViewThread::setScreenDimensions(int w,int h) {
	QMutexLocker locker(&d->m_mutex);
	if (d->m_screen_size!=QSize(w,h)) {
		d->m_need_to_rerender=true;
		d->m_screen_size=QSize(w,h);
	}
}
void SquidViewThread::setScreenRect(const QRectF &R) {
	QMutexLocker locker(&d->m_mutex);
	if (d->m_screen_rect!=R) {
		d->m_need_to_rerender=true;
		d->m_screen_rect=R;
	}
}
void SquidViewThread::setBackgroundColor(QColor col) {
	QMutexLocker locker(&d->m_mutex);
	d->m_background_color=col;
}
QRectF SquidViewThread::screenRect() const {
	QMutexLocker locker(&d->m_mutex);
	return d->m_screen_rect;
}
QImage SquidViewThread::image() const {
	QMutexLocker locker(&d->m_mutex);
	return d->m_image;
}

void SquidViewThread::run() {
	bool halt_thread;
	d->m_mutex.lock();
	halt_thread=d->m_halt_thread;
	int total_screen_size=d->m_screen_size.width()*d->m_screen_size.height();
	d->m_mutex.unlock();
	if (halt_thread) {
		d->m_mutex.lock();
		d->m_halt_thread=false;
		d->m_mutex.unlock();
		return;
	}
	if (total_screen_size<=5) return;
	
	bool done=false;
	while (!done) {
		d->m_mutex.lock();
		d->m_need_to_rerender=false;
		int w=d->m_screen_size.width();
		int h=d->m_screen_size.height();
		SquidTransform tmp_transform=d->m_transform;
		int object_count=d->m_objects.count();
		d->m_mutex.unlock();
		
		QList<float> zvalues;
		for (int i=0; i<object_count; i++) {
			d->m_mutex.lock();
			halt_thread=d->m_halt_thread;
			SquidObject *obj=d->m_objects[i];
			zvalues << obj->zValue(tmp_transform);
			d->m_mutex.unlock();
			if (halt_thread) {
				d->m_mutex.lock();
				d->m_halt_thread=false;
				d->m_mutex.unlock();
				return;
			}
		}
		QList<int> sorted_indices=get_sorted_indices(zvalues);

				
		if (sorted_indices.count()>3) {
			for (int i=0; i<sorted_indices.count(); i++) {
				float pct=i*1.0/sorted_indices.count();
				d->m_objects[sorted_indices[i]]->setShadingPct(pct);
			}
		}
		else {
			for (int i=0; i<sorted_indices.count(); i++) {
				d->m_objects[sorted_indices[i]]->setShadingPct(1);
			}
		}
		
		QColor background_color;
		d->m_mutex.lock();
		halt_thread=d->m_halt_thread;
		background_color=d->m_background_color;
		d->m_mutex.unlock();
		if (halt_thread) {
			d->m_mutex.lock();
			d->m_halt_thread=false;
			d->m_mutex.unlock();
			return;
		}
			
		QTime timer;
		timer.start();

		int num_stages=4;
		int stage=1;
		bool continue_with_stages=true;
		while ((stage<=num_stages)&&(continue_with_stages)) {
			d->m_mutex.lock();
			QImage tmp_image(w,h,QImage::Format_RGB32);
			QPainter painter(&tmp_image);
			painter.setRenderHint(QPainter::Antialiasing);
			painter.fillRect(0,0,w,h,background_color);
			d->m_mutex.unlock();
			
			int begin_ind=0;
			int end_ind=sorted_indices.count()-1;
			if (stage==1) begin_ind=qMax(0,end_ind-10000);
			else if (stage==2) begin_ind=qMax(0,end_ind-30000);
			else if (stage==3) begin_ind=qMax(0,end_ind-60000);
			else if (stage==4) begin_ind=0;
			bool done_early=false;
			for (int i=begin_ind; (i<=end_ind)&&(!done_early); i++) {
				d->m_mutex.lock();
				halt_thread=d->m_halt_thread;
				d->m_mutex.unlock();
				if (halt_thread) {
					d->m_mutex.lock();
					d->m_halt_thread=false;
					d->m_mutex.unlock();
					return;
				}
				
				d->m_mutex.lock();
				if ((d->m_need_to_rerender)&&(stage>1)) done_early=true;
				SquidObject *obj=d->m_objects[sorted_indices[i]];
				obj->render(painter,d->m_screen_rect,tmp_transform);
				d->m_mutex.unlock();
			}
			if (!done_early) {
				d->m_mutex.lock();
				d->m_image=tmp_image;
				d->m_mutex.unlock();
				emit updated();
			}
			
			d->m_mutex.lock();	
			if (d->m_need_to_rerender) continue_with_stages=false;
			if (begin_ind==0) continue_with_stages=false;
			d->m_mutex.unlock();
			stage++;
		}		
		
		
		d->m_mutex.lock();
		if (d->m_need_to_rerender) done=false;
		else done=true;
		d->m_mutex.unlock();
	}
	d->m_mutex.lock();
	d->m_halt_thread=false;
	d->m_mutex.unlock();
}
void SquidViewThread::lockThread() {
	d->m_mutex.lock();
}
void SquidViewThread::unlockThread() {
	d->m_mutex.unlock();
}
void SquidViewThread::setTransform(const SquidTransform &T) {
	QMutexLocker locker(&d->m_mutex);
	if (!isEqual(d->m_transform,T)) {
		copyTransform(d->m_transform,T);
		d->m_need_to_rerender=true;
	}
}
SquidTransform SquidViewThread::transform() const {
	QMutexLocker locker(&d->m_mutex);
	return d->m_transform;
}
void SquidViewThread::haltThread() {
	QMutexLocker locker(&d->m_mutex);
	if (isRunning()) d->m_halt_thread=true;
}


class SquidViewPrivate {
public:
	SquidViewBase *q;
	SquidViewThread m_thread;
	int m_num_objects_rendered;
	SquidTransform m_transform;
	SquidViewBaseHelper *m_helper;
	
	void determine_render_order();
};


SquidViewBase::SquidViewBase() {
	d=new SquidViewPrivate;
	d->q=this;
	d->m_helper=new SquidViewBaseHelper;
	d->m_helper->SVI=this;
	setIdentity(d->m_transform);
	QObject::connect(&d->m_thread,SIGNAL(updated()),d->m_helper,SLOT(slot_updated()),Qt::DirectConnection);
}
SquidViewBase::~SquidViewBase() {
	delete d->m_helper;
	delete d;
}
int SquidViewBase::objectCount() const {
	return d->m_thread.objectCount();
}
SquidObject *SquidViewBase::object(int index) const {
	return d->m_thread.object(index);
}
void SquidViewBase::addObject(SquidObject *obj) {
	d->m_thread.addObject(obj);
}
void SquidViewBase::clearObjects() {
	d->m_thread.haltThread();
	d->m_thread.wait(1000);
	if (d->m_thread.isRunning()) {
		qWarning() << "TERMINATING THREAD!";
		d->m_thread.terminate();
	}
	d->m_thread.clearObjects();
}
void SquidViewBase::setTransform(const SquidTransform &T) {
	copyTransform(d->m_transform,T);
}
SquidTransform SquidViewBase::transform() const {
	return d->m_transform;
}
void SquidViewBaseHelper::slot_updated() {
	SVI->updateScreen();
}
void SquidViewBase::refresh(bool run_in_separate_thread) {
	d->m_thread.setScreenDimensions(screenWidth(),screenHeight());
	d->m_thread.setScreenRect(QRectF(-screenWidth()/2.0F,-screenHeight()/2.0F,screenWidth(),screenHeight()));
	d->m_thread.setTransform(d->m_transform);
	if ((screenWidth()>1)&&(screenHeight()>1)) {
		if (run_in_separate_thread)
			d->m_thread.start();
		else
			d->m_thread.run();
	}
}
QImage SquidViewBase::image() const {
	return d->m_thread.image();
}
bool SquidViewBase::loadView(QString fname) {
	clearObjects();
	setIdentity(d->m_transform);	
	
	QFile inf(fname);
	if (!inf.open(QIODevice::ReadOnly)) return false;
	QDataStream in(&inf);
	in.setVersion(QDataStream::Qt_4_5);
	
	//skip 128 byte header
	quint8 dummy=0;
	for (int i=0; i<128; i++) in >> dummy;
	
	bool done=false;
	while ((!done)&&(!inf.atEnd())) {
		qint32 section_type;
		in >> section_type;
		if (section_type>0) {
			qint64 section_size;
			in >> section_size;
			
			if (section_type==SQV_OBJECTS) {
				qint64 num_objects;
				in >> num_objects;
				qDebug() << "# objects = " << num_objects;
				for (qint64 i=0; i<num_objects; i++) {
					qint32 object_type;
					qint32 save_size;
					in >> object_type;
					in >> save_size;
					SquidObject *obj=create_object_by_type(object_type);
					if (obj) {
						obj->loadObject(inf,save_size);
						addObject(obj);
					}
					else {
						for (int i=0; i<save_size; i++)
							in >> dummy;
					}
				}
			}
			else if (section_type==SQV_TRANSFORM) {
				for (int j=0; j<4; j++)
				for (int i=0; i<4; i++) {
					float val;
					in >> val;
					d->m_transform.data[i][j]=val;
				}
			}
			else {
				for (long i=0; i<section_size; i++) {
					in >> dummy;
				}
			}
		}
		else done=true;
	}	
	inf.close();
	return true;
}
bool SquidViewBase::saveView(QString fname) {
	QFile outf(fname);
	if (!outf.open(QIODevice::WriteOnly)) return false;
	QDataStream out(&outf);
	out.setVersion(QDataStream::Qt_4_5);
	
	//first 128 bytes are header, first 4 of which say the version
	qint32 version=1;
	out << version;
	quint8 dummy=0;
	for (int i=0; i<128-4; i++) out << dummy;
	
	//all remaining sections have this format: 
	// * 4 bytes for the section type (0 means done)
	// * 8 bytes for the length of the section data
	// * section data
	
	//SQV_OBJECTS
	{
		qint64 total_size=0;
		for (int i=0; i<d->m_thread.objectCount(); i++) {
			SquidObject *obj=d->m_thread.object(i);
			if (obj->objectType()>0) {
				total_size+=4+4+obj->saveObjectSize();
			}
		}
		qint32 section_type=SQV_OBJECTS;
		qint64 section_size=total_size;
		out << section_type;
		out << section_size;
		qint64 num_objects=0;
		for (int i=0; i<d->m_thread.objectCount(); i++) {
			SquidObject *obj=d->m_thread.object(i);
			if (obj->objectType()>0) {
				num_objects++;
			}
		}
		out << num_objects;
		for (int i=0; i<d->m_thread.objectCount(); i++) {
			SquidObject *obj=d->m_thread.object(i);
			if (obj->objectType()>0) {
				qint32 object_type=obj->objectType();
				qint32 save_size=obj->saveObjectSize();
				out << object_type;
				out << save_size;
				obj->saveObject(outf);
			}
		}
	}
	
	//SQV_TRANSFORM
	{
		qint32 section_type=SQV_TRANSFORM;
		qint64 section_size=16*4;
		out << section_type;
		out << section_size;
		for (int j=0; j<4; j++)
		for (int i=0; i<4; i++) {
			float val=d->m_transform.data[i][j];
			out << val;
		}
	}
	
	//end
	qint32 zero=0;
	out << zero << zero;
	
	outf.close();
	
	return true;
}

SquidObject *SquidViewBase::objectAtPoint(QPoint pt) const {
	QList<SquidObject *> list;
	for (int i=0; i<objectCount(); i++) {
		SquidObject *obj=object(i);
		if (obj->containsPoint(pt,d->m_thread.screenRect(),d->m_transform))
			list << obj;
	}
	SquidObject * ret=0;
	float bestz=0;
	foreach (SquidObject *obj,list) {
		if ((!ret)||(obj->zValue(d->m_transform)>bestz)) {
			ret=obj;
			bestz=obj->zValue(d->m_transform);
		}
	}
	return ret;
}
void SquidViewBase::setBackgroundColor(QColor col) {
	d->m_thread.setBackgroundColor(col);
}
