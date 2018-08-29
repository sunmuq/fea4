#ifndef median_filter_H
#define median_filter_H

/****************************************************************************************
median_filter() - Two dimensional median filter

Author: Jeremy Magland, June 2009
Notes: Works well for relatively small values of radius (e.g. between 1 and 10)
Input:
  MxN are the dimensions of the image
  input is the input array of size MxN
  mask is an optional mask array of size MxN (use mask=NULL for no mask)
  radius is the radius for the median filter -- kernel size will be (2*radius+1)x(2*radius+1)
Output:
  output is the output array of size MxN (must be preallocated)
*****************************************************************************************/
#include <QImage>
void median_filter(int M,int N,unsigned char *output,unsigned char *input,unsigned char *mask,int radius);
QImage median_filter(const QImage &img,unsigned char *mask,int radius);

/****************************************************************************************
median_filter_3d() - Three dimensional median filter

Input:
  MxNxP are the dimensions of the image
  input is the input array of size MxNxP
  mask is an optional mask array of size MxNxP (use mask=NULL for no mask)
  radius is the radius for the median filter -- kernel size will be (2*radius+1)x(2*radius+1)x(2*radius+1)
Output:
  output is the output array of size MxNxP (must be preallocated)
*****************************************************************************************/
void median_filter_3d(int M,int N,int P,unsigned char *output,unsigned char *input,unsigned char *mask,int radius);

#endif
