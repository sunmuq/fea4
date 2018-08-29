#include "squidarray2d.h"
#include <stdlib.h>

class SquidArray2DPrivate {
public:
	float *m_data;
	int m_width;
	int m_height;
	
	void copy_from(const SquidArray2D &X);
	void allocate(int w,int h);
};

void SquidArray2DPrivate::copy_from(const SquidArray2D &X) {	
	allocate(X.width(),X.height());
	for (long ii=0; ii<X.width()*X.height(); ii++)
		m_data[ii]=X.value(ii);
}
void SquidArray2DPrivate::allocate(int w,int h) {
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

SquidArray2D::SquidArray2D() {
	d=new SquidArray2DPrivate;
	d->m_data=0;
	d->m_width=0;
	d->m_height=0;
}
SquidArray2D::SquidArray2D(const SquidArray2D &X) {
	d=new SquidArray2DPrivate;
	d->m_data=0;
	d->m_width=0;
	d->m_height=0;
	d->copy_from(X);	
}
SquidArray2D::~SquidArray2D() {
	clear();
	delete d;
}
void SquidArray2D::clear() {
	d->allocate(0,0);
}
void SquidArray2D::allocate(int w,int h) {
	d->allocate(w,h);
}
int SquidArray2D::width() const {
	return d->m_width;
}
int SquidArray2D::height() const {
	return d->m_height;
}
float SquidArray2D::value(int i,int j) const {
	return d->m_data[i+width()*j];
}
void SquidArray2D::setValue(float val,int i,int j) {
	d->m_data[i+width()*j]=val;
}
void SquidArray2D::setAll(float val) {
	for (long ii=0; ii<width()*height(); ii++)
		d->m_data[ii]=val;
}
void SquidArray2D::operator=(const SquidArray2D &X) {
	d->copy_from(X);
}
void SquidArray2D::getData(float *ret) const {
	long N=d->m_width*d->m_height;
	for (long ii=0; ii<N; ii++)
		ret[ii]=d->m_data[ii];
}
void SquidArray2D::setData(float *D) {
	long N=d->m_width*d->m_height;
	for (long ii=0; ii<N; ii++)
		d->m_data[ii]=D[ii];
}
