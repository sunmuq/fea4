#ifndef sinc_interpolate_H
#define sinc_interpolate_H

#include "fftw3.h"

//input must allocated using fftw_malloc of size N1xN2xN3
//output must be preallocated of size (N1*M1)x(N2*M2)x(N3*M3)
void sinc_interpolate(double *output,long N1,long N2,long N3,double *input,int F1,int F2,int F3);

struct sinc_resample_affine_params {
	/*dimensions of output*/
	long oN1;
	long oN2;
	long oN3;
	/*output array*/
	double *output; //output must pre-allocated of size oN1xoN2xoN3
	
	/*dimensions of input*/
	long iN1;
	long iN2;
	long iN3;
	/*input array*/
	double *input; //input must allocated using fftw_malloc of size iN1xiN2xiN3
	
	/*oversampling factors*/
	long F1;
	long F2;
	long F3;
	
	/*affine transformation matrix*/
	double transformation[4][4];
};
void sinc_resample_affine(sinc_resample_affine_params &P);

#endif
