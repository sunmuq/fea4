#ifndef array2d_H
#define array2d_H

#include <QString>
#include <QList>
#include <QDebug>
#ifdef USING_JARCHIVE
#include "jarchive.h"
#endif

class Array2DPrivate;

class Array2D {
public:
	friend class Array2DPrivate;
	Array2D(int W=0,int H=0);
	Array2D(const Array2D &X);
	virtual ~Array2D();
	void clear();
	void allocate(int w,int h);
	int width() const;
	int height() const;
	float value(int i,int j) const;
	float value1(int i) const;
	void setValue(float val,int i,int j);
	void incrementValue(float val,int i,int j);
	void setAll(float val);
	void getData(float *ret) const;
	void setData(float *D);
	void read(QString mda_fname);
	void write(QString mda_fname) const;
	void readSlice(QString mda_fname,int slice_number);
	void operator=(const Array2D &X);
	float max() const;
	float min() const;
	QList<float> dataY(int x) const;
	QList<float> dataX(int y) const;
	void setDataX(const QList<float> &X,int y);
	void setDataY(const QList<float> &X,int x);
	void scaleBy(float val);
	void add(const Array2D &X);
	
	#ifdef USING_JARCHIVE
	void write(JArchive &A,QString key);
	void read(JArchive &A,QString key);
	#endif
private:
	Array2DPrivate *d;
};



void write_mda(const QString &fname,const QList<float> &data);
void read_mda(const QString &fname,QList<float> &data);
QList<float> read_float_list(const QString &fname);
void add(QList<float> &X,const QList<float> &Y);

#endif

