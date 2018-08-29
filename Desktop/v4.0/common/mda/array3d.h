#ifndef array3d_H
#define array3d_H

#include <QList>
#include "array2d.h"

#ifdef USING_JARCHIVE
#include "jarchive.h"
#endif

#define STORAGE_ORDER_XYZ 1
#define STORAGE_ORDER_XZY 2
#define STORAGE_ORDER_YXZ 3
#define STORAGE_ORDER_YZX 4
#define STORAGE_ORDER_ZXY 5
#define STORAGE_ORDER_ZYX 6

class Array3DPrivate;

class Array3D {
public:
	friend class Array3DPrivate;
	Array3D(int W=0,int H=0,int S=0,int storage_order=STORAGE_ORDER_XYZ);
	Array3D(const Array3D &X);
	virtual ~Array3D();
	void clear();
	void allocate(int w,int h,int N,int storage_order=STORAGE_ORDER_XYZ);
	int width() const;
	int height() const;
	int numSlices() const;
	int storageOrder() const;
	float value(int i,int j,int k) const;
	float value1(int i) const;
	void setValue(float val,int i,int j,int k);
	void incrementValue(float val,int i,int j,int k);
	void setAll(float val);
	void read(QString mda_fname,int storage_order=STORAGE_ORDER_XYZ);
	void write(QString mda_fname) const;
	void writeInt16(QString mda_fname) const;
	void operator=(const Array3D &X);
	QList<float> dataX(int y,int z) const;
	QList<float> dataY(int x,int z) const;
	QList<float> dataZ(int x,int y) const;
	Array2D dataXY(int z) const;
	Array2D dataXZ(int y) const;
	Array2D dataYZ(int x) const;
	Array2D dataYX(int z) const;
	Array2D dataZX(int y) const;
	Array2D dataZY(int x) const;
	void setDataXY(const Array2D &X,int z);
	void setDataX(const QList<float> &X,int y,int z);
	void setDataY(const QList<float> &X,int x,int z);
	void setDataZ(const QList<float> &X,int x,int y);
	float max() const;
	float min() const;
	void scaleBy(float val);
	
	#ifdef USING_JARCHIVE
	void write(JArchive &A,QString key);
	void read(JArchive &A,QString key);
	#endif
private:
	Array3DPrivate *d;
};

#endif
