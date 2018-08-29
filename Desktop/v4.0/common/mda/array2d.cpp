#include "array2d.h"
#include <stdlib.h>
#include "mda.h"

class Array2DPrivate {
public:
	float *m_data;
	int m_width;
	int m_height;
	
	void copy_from(const Array2D &X);
	void allocate(int w,int h);
};

void Array2DPrivate::copy_from(const Array2D &X) {	
	allocate(X.width(),X.height());
	for (long ii=0; ii<X.width()*X.height(); ii++)
		m_data[ii]=X.d->m_data[ii];
}
void Array2DPrivate::allocate(int w,int h) {
	if (m_data) free(m_data);
	if (w*h>0) {
		m_data=(float *)malloc(sizeof(float)*w*h);
		for (long ii=0; ii<w*h; ii++) m_data[ii]=0;
	}
	else {
		m_data=0;
	}
	m_width=w;
	m_height=h;
}

Array2D::Array2D(int W,int H) {
	d=new Array2DPrivate;
	d->m_data=0;
	d->m_width=0;
	d->m_height=0;
	if (W*H>0) allocate(W,H);
}
Array2D::Array2D(const Array2D &X) {
	d=new Array2DPrivate;
	d->m_data=0;
	d->m_width=0;
	d->m_height=0;
	d->copy_from(X);	
}
Array2D::~Array2D() {
	clear();
	delete d;
}
void Array2D::clear() {
	d->allocate(0,0);
}
void Array2D::allocate(int w,int h) {
	d->allocate(w,h);
}
int Array2D::width() const {
	return d->m_width;
}
int Array2D::height() const {
	return d->m_height;
}
float Array2D::value(int i,int j) const {
	if ((i<0)||(i>=d->m_width)||(j<0)||(j>=d->m_height)) return 0;
	return d->m_data[i+width()*j];
}
void Array2D::setValue(float val,int i,int j) {
	if ((i<0)||(i>=d->m_width)||(j<0)||(j>=d->m_height)) return;
	d->m_data[i+width()*j]=val;
}
void Array2D::incrementValue(float val,int i,int j) {
	if ((i<0)||(i>=d->m_width)||(j<0)||(j>=d->m_height)) return;
	d->m_data[i+width()*j]+=val;
}
void Array2D::setAll(float val) {
	for (long ii=0; ii<width()*height(); ii++)
		d->m_data[ii]=val;
}
void Array2D::operator=(const Array2D &X) {
	d->copy_from(X);
}
void Array2D::getData(float *ret) const {
	long N=d->m_width*d->m_height;
	for (long ii=0; ii<N; ii++)
		ret[ii]=d->m_data[ii];
}
void Array2D::setData(float *D) {
	long N=d->m_width*d->m_height;
	for (long ii=0; ii<N; ii++)
		d->m_data[ii]=D[ii];
}
float Array2D::value1(int i) const {
	return d->m_data[i];
}
void Array2D::read(QString mda_fname) {
	#ifdef WIN32
	mda_fname.replace("/","\\");
	#endif
	Mda M;
	M.read(mda_fname.toAscii().data());
	allocate(M.size(0),M.size(1));
	for (int y=0; y<height(); y++)
	for (int x=0; x<width(); x++) {
		setValue(M(x,y).re(),x,y);
	}
}
void Array2D::readSlice(QString mda_fname,int slice_number) {
	#ifdef WIN32
	mda_fname.replace("/","\\");
	#endif
	Mda M;
	M.readSlices(mda_fname.toAscii().data(),slice_number,slice_number);
	allocate(M.size(0),M.size(1));
	for (int y=0; y<height(); y++)
	for (int x=0; x<width(); x++) {
		setValue(M(x,y).re(),x,y);
	}
}
void Array2D::write(QString mda_fname) const {
	#ifdef WIN32
	mda_fname.replace("/","\\");
	#endif
	Mda M;
	M.allocate(MDA_TYPE_REAL,width(),height());
	for (int y=0; y<height(); y++)
	for (int x=0; x<width(); x++) {
		M(x,y)=this->value(x,y);
	}
	M.write(mda_fname.toAscii().data());
}
void write_mda(const QString &fname2,const QList<float> &data) {
	QString fname=fname2;
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	Mda M;
	M.allocate(MDA_TYPE_REAL,data.count(),1);
	for (int x=0; x<data.count(); x++) {
		M(x)=data[x];
	}
	M.write(fname.toAscii().data());
}
void read_mda(const QString &fname2,QList<float> &data) {
	QString fname=fname2;
	data.clear();
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	Mda M;
	M.read(fname.toAscii().data());
	for (int x=0; x<M.size(0); x++) {
		data << M(x).re();
	}
}
QList<float> read_float_list(const QString &fname) {
	QList<float> X;
	read_mda(fname,X);
	return X;
}
float Array2D::max() const {
	float ret=0;
	bool first=true;
	for (int y=0; y<height(); y++)
	for (int x=0; x<width(); x++) {
		float val=value(x,y);
		if ((first)||(val>ret)) ret=val;
		first=false;
	}
	return ret;
}
float Array2D::min() const {
	float ret=0;
	bool first=true;
	for (int y=0; y<height(); y++)
	for (int x=0; x<width(); x++) {
		float val=value(x,y);
		if ((first)||(val<ret)) ret=val;
		first=false;
	}
	return ret;
}
QList<float> Array2D::dataY(int x) const {
	QList<float> ret;
	for (int y=0; y<d->m_height; y++)
		ret << value(x,y);
	return ret;
}
QList<float> Array2D::dataX(int y) const {
	QList<float> ret;
	for (int x=0; x<d->m_width; x++)
		ret << value(x,y);
	return ret;
}
void Array2D::setDataX(const QList<float> &X,int y) {
	for (int x=0; x<width(); x++)
		setValue(X.value(x),x,y);
}
void Array2D::setDataY(const QList<float> &X,int x) {
	for (int y=0; y<height(); y++)
		setValue(X.value(y),x,y);
}


#ifdef USING_JARCHIVE
void Array2D::write(JArchive &A,QString key) {
	A.beginGroup(key);
	A.writeValue("width",width());
	A.writeValue("height",height());
	A.writeFloatArray("data",d->m_data,width()*height());
	A.endGroup();
}
void Array2D::read(JArchive &A,QString key) {
	A.beginGroup(key);
	int W=A.readValue("width").toInt();
	int H=A.readValue("height").toInt();
	allocate(W,H);
	A.readFloatArray("data",d->m_data,width()*height());
	A.endGroup();
}
#endif
void Array2D::scaleBy(float val) {
	for (int j=0; j<d->m_width*d->m_height; j++)
		d->m_data[j]*=val;
}
void Array2D::add(const Array2D &X) {
	for (int y=0; y<height(); y++)
	for (int x=0; x<width(); x++) {
		setValue(value(x,y)+X.value(x,y),x,y);
	}
}
void add(QList<float> &X,const QList<float> &Y) {
	for (int i=0; i<X.count(); i++)
		X[i]+=Y[i];
}
