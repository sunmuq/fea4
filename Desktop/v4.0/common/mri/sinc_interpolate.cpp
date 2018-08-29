#include "sinc_interpolate.h"
#include <stdlib.h>
#include <math.h>
#include <QDebug>

class SincInterpolateHandler {
public:
	long N1,N2,N3;
	long F1,F2,F3;
	virtual void initialize() {}
	virtual void handle(double *X,int x,int y,int z) {
		x=y=z=0;
		X=0;
	}
};

void fft(double *output,long N1,long N2,long N3,double *input) {
	int n[1]; n[0]=N1;
	fftw_r2r_kind kind=FFTW_R2HC;
	
	fftw_plan p=fftw_plan_many_r2r(/*rank*/1, n, N2*N3,
                                  input, /*inembed=*/NULL,
                                  /*istride=*/1, /*idist=*/N1,
                                  output, /*onembed=*/NULL,
                                  /*ostride=*/1, /*odist=*/N1,
                                  &kind, FFTW_ESTIMATE);     
	fftw_execute(p);                                       
	fftw_destroy_plan(p);
}
void ifft(double *output,long N1,long N2,long N3,double *input) {
	int n[1]; n[0]=N1;
	fftw_r2r_kind kind[1]; kind[0]=FFTW_HC2R;
	
	fftw_plan p=fftw_plan_many_r2r(/*rank*/1, n, N2*N3,
                                  input, /*inembed=*/NULL,
                                  /*istride=*/1, /*idist=*/N1,
                                  output, /*onembed=*/NULL,
                                  /*ostride=*/1, /*odist=*/N1,
                                  kind, FFTW_ESTIMATE);     
	fftw_execute(p);                                       
	for (long j=0; j<N1*N2*N3; j++)
		output[j]/=N1;
	fftw_destroy_plan(p);
}
void transpose12(double *output,long N1,long N2,long N3,double *input) {
	long ct=0;
	for (long l=0; l<N3; l++)	
	for (long k=0; k<N2; k++)
	for (long j=0; j<N1; j++) {
		output[k+N2*j+N1*N2*l]=input[ct];
		ct++;
	}
}
void transpose13(double *output,long N1,long N2,long N3,double *input) {
	long ct=0;
	for (long l=0; l<N3; l++)	
	for (long k=0; k<N2; k++)
	for (long j=0; j<N1; j++) {
		output[l+N3*k+N2*N3*j]=input[ct];
		ct++;
	}
}
void shift(long N1,long N2,long N3,double *X,double fraction) {
	double *hold=(double *)malloc(sizeof(double)*N1);
	for (long j=0; j<N2*N3; j++) {
		long ind=j*N1;
		for (long k=0; k<N1; k++)
			hold[k]=X[ind+k];
		//X[ind+0]=hold[0]; //leave it alone
		double factor=2*3.141592653589793/N1*fraction;
		for (long k=1; k<N1/2; k++) {
			double th=factor*k;
			X[ind+k]=hold[k]*cos(th)-hold[N1-k]*sin(th);
			X[ind+N1-k]=hold[k]*sin(th)+hold[N1-k]*cos(th);
		}
		if (N1%2==0) {
			//X[ind+N1/2]=hold[N1/2]; //leave it alone
		}
	}
	free(hold);
}

//input must allocated using fftw_malloc of size N1xN2xN3
void handle_sinc_interpolation(SincInterpolateHandler *H,double *input) {
	H->initialize();
	long N1=H->N1; long N2=H->N2; long N3=H->N3;
	long F1=H->F1; long F2=H->F2; long F3=H->F3;
	double *hold=(double *)fftw_malloc(sizeof(double)*N1*N2*N3);
	int x=0;
	int y=0; 
	int z=0;
	int xdir=+1;
	int ydir=+1;
	bool done=false;
	while (!done) {
		printf("handle_sinc_interpolation: %d,%d,%d\n",x,y,z);
		H->handle(input,x,y,z);
		int endx=F1-1; if (xdir==-1) endx=0;
		int endy=F2-1; if (ydir==-1) endy=0;
		int endz=F3-1;
		if (x==endx) { //reached end of X
			if (y==endy) { //reached end of Y
				if (z==endz) { //reached end of Z
					done=true;
				}
				else {
					transpose13(hold,N1,N2,N3,input);
					fft(hold,N3,N2,N1,hold);
					shift(N3,N2,N1,hold,1.0/F3);
					ifft(hold,N3,N2,N1,hold);
					transpose13(input,N3,N2,N1,hold);
					z++;
				}
				ydir=-ydir;
			}
			else {
				transpose12(hold,N1,N2,N3,input);
				fft(hold,N2,N1,N3,hold);
				shift(N2,N1,N3,hold,ydir*1.0/F2);
				ifft(hold,N2,N1,N3,hold);
				transpose12(input,N2,N1,N3,hold);
				y+=ydir;
			}
			xdir=-xdir;
		}
		else {
			if (((xdir==1)&&(x==0)) || ((xdir==-1)&&(x==F1-1))) {
				fft(hold,N1,N2,N3,input);
			}
			shift(N1,N2,N3,hold,xdir*1.0/F1);
			ifft(input,N1,N2,N3,hold);
			x+=xdir;
		}
	}
	fftw_free(hold);
}

class sinc_interpolate_handler : public SincInterpolateHandler {
public:
	double *output;
	void initialize() {}
	void handle(double *X,int x,int y,int z) {
		long ct=0;
		for (long l=0; l<N3; l++)
		for (long k=0; k<N2; k++)
		for (long j=0; j<N1; j++) {
			long ind=(j*F1+x)+F1*N1*(k*F2+y)+F1*N1*F2*N2*(l*F3+z);
			output[ind]=X[ct];
			ct++;
		}
	}
};

//input must allocated using fftw_malloc of size N1xN2xN3
//output must be preallocated of size (N1*F1)x(N2*F2)x(N3*F3)
void sinc_interpolate(double *output,long N1,long N2,long N3,double *input,int F1,int F2,int F3) {
	sinc_interpolate_handler H;
	H.output=output;
	H.N1=N1; H.N2=N2; H.N3=N3; 
	H.F1=F1; H.F2=F2; H.F3=F3;
	handle_sinc_interpolation(&H,input);
}

class sinc_resample_affine_handler : public SincInterpolateHandler {
public:
	double *output;
	long oN1,oN2,oN3;
	double transformation[4][4];
	void initialize() {
		for (long j=0; j<oN1*oN2*oN3; j++)
			output[j]=0;
	}
	void handle(double *X,int x,int y,int z) {
		for (long i1=0; i1<oN1; i1++)
		for (long i2=0; i2<oN2; i2++)
		for (long i3=0; i3<oN3; i3++) {
			double xx,yy,zz;
			transform(xx,yy,zz,i1,i2,i3);
			long ii1=(long)(xx); 
			long ii2=(long)(yy);
			long ii3=(long)(zz);
			if ((x==0)&&(xx+1.0/F1>=ii1+1)) ii1++;
			if ((y==0)&&(yy+1.0/F2>=ii2+1)) ii2++;
			if ((z==0)&&(zz+1.0/F3>=ii3+1)) ii3++;
			if ((0<=ii1)&&(ii1<N1)&&(0<=ii2)&&(ii2<N2)&&(0<=ii3)&&(ii3<N3)) {
				double dx=(xx-ii1)*F1-x;
				double dy=(yy-ii2)*F2-y;
				double dz=(zz-ii3)*F3-z;
				double factor=1;
				if ((0<=dx)&&(dx<1.0)) {
					factor*=(1-dx);
					if ((0<=dy)&&(dy<1.0)) {
						factor*=(1-dy);
						if ((0<=dz)&&(dz<1.0)) {
							factor*=(1-dz);
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
						else if ((-1.0<dz)&&(dz<0)) {
							factor*=1+dz;
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
					}
					else if ((-1.0<dy)&&(dy<0)) {
						factor*=1+dy;
						if ((0<=dz)&&(dz<1.0)) {
							factor*=(1-dz);
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
						else if ((-1.0<dz)&&(dz<0)) {
							factor*=1+dz;
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
					}
				}
				else if ((-1.0<dx)&&(dx<0)) {
					factor*=1+dx;
					if ((0<=dy)&&(dy<1.0)) {
						factor*=(1-dy);
						if ((0<=dz)&&(dz<1.0)) {
							factor*=(1-dz);
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
						else if ((-1.0<dz)&&(dz<0)) {
							factor*=1+dz;
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
					}
					else if ((-1.0<dy)&&(dy<0)) {
						factor*=1+dy;
						if ((0<=dz)&&(dz<1.0)) {
							factor*=(1-dz);
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
						else if ((-1.0<dz)&&(dz<0)) {
							factor*=1+dz;
							output[i1+oN1*i2+oN1*oN2*i3]+=X[ii1+N1*ii2+N1*N2*ii3]*factor;
						}
					}
				}
			}
		}
	}
private:
	void transform(double &ox,double &oy,double &oz,double ix,double iy,double iz) {
		ox=transformation[0][0]*ix+transformation[0][1]*iy+transformation[0][2]*iz+transformation[0][3];
		oy=transformation[1][0]*ix+transformation[1][1]*iy+transformation[1][2]*iz+transformation[1][3];
		oz=transformation[2][0]*ix+transformation[2][1]*iy+transformation[2][2]*iz+transformation[2][3];
	}
};

void sinc_resample_affine(sinc_resample_affine_params &P) {
	sinc_resample_affine_handler H;
	H.output=P.output;
	H.oN1=P.oN1; H.oN2=P.oN2; H.oN3=P.oN3;
	for (int j=0; j<4; j++)
	for (int k=0; k<4; k++)
		H.transformation[j][k]=P.transformation[j][k];
	H.N1=P.iN1; H.N2=P.iN2; H.N3=P.iN3; 
	H.F1=P.F1; H.F2=P.F2; H.F3=P.F3;
	handle_sinc_interpolation(&H,P.input);
}

