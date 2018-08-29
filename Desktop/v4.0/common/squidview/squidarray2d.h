#ifndef squidarray2d_H
#define squidarray2d_H

class SquidArray2DPrivate;
class SquidArray2D {
public:
	friend class SquidArray2DPrivate;
	SquidArray2D();
	SquidArray2D(const SquidArray2D &X);
	virtual ~SquidArray2D();
	void clear();
	void allocate(int w,int h);
	int width() const;
	int height() const;
	float value(int i,int j=0) const;
	void setValue(float val,int i,int j=0);
	void setAll(float val);
	void getData(float *ret) const;
	void setData(float *D);
	void operator=(const SquidArray2D &X);
private:
	SquidArray2DPrivate *d;
};

#endif
