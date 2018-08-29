#include "median_filter.h"
#include <stdio.h>
#include <QDebug>
#include <QColor>

class MFHistogram {
public:
	MFHistogram();
	void add(unsigned char val);
	void remove(unsigned char val);
	unsigned char median();
	int size() {return m_num_values;}
	void clear();
private:
	int m_data[256];
	int m_num_values;
	unsigned char m_median;
	int m_num_values_below_median;
	
	void move_median_down();
	void move_median_up();
	unsigned char average_of_median_and_previous_value();
};

MFHistogram::MFHistogram() {
	for (int j=0; j<256; j++) m_data[j]=0;
	m_num_values=0;
	m_num_values_below_median=0;
	m_median=0;
}
void MFHistogram::add(unsigned char val) {
	m_data[val]++;
	m_num_values++;
	if (m_num_values==1) {
		m_median=val;
		m_num_values_below_median=0;
	}
	else if (val==m_median) {
		//nothing to do
	}
	else if (val>m_median) {
		int midindex=m_num_values/2;
		if (m_num_values_below_median+m_data[m_median]-1<midindex) {
			move_median_up();
		}
	}
	else if (val<m_median) {
		m_num_values_below_median++;
		int midindex=m_num_values/2;
		if (m_num_values_below_median>midindex) {
			move_median_down();
		}
	}
	
}
void MFHistogram::remove(unsigned char val) {
	if (!m_data[val]) {
		qWarning() << "Unable to remove value from histogram:" << val;
		return;
	}
	m_data[val]--;
	m_num_values--;
	if (m_num_values==0) {
		m_median=0;
		m_num_values_below_median=0;
	}
	else if (val==m_median) {
		int midindex=m_num_values/2;
		if (m_num_values_below_median>midindex) {
			move_median_down();
		}
		if (m_num_values_below_median+m_data[m_median]-1<midindex) {
			move_median_up();
		}
	}
	else if (val>m_median) {
		int midindex=m_num_values/2;
		if (m_num_values_below_median>midindex) {
			move_median_down();
		}
	}
	else if (val<m_median) {
		m_num_values_below_median--;
		int midindex=m_num_values/2;
		if (m_num_values_below_median+m_data[m_median]-1<midindex) {
			move_median_up();
		}
	}
}
void MFHistogram::clear() {
	m_num_values=0;
	for (int i=0; i<256; i++)
		m_data[i]=0;
	m_median=0;
	m_num_values_below_median=0;
}
unsigned char MFHistogram::median() {
	if (m_num_values%2==0) { //tricky case of even number of values
		if (m_num_values_below_median==m_num_values/2) {
			return average_of_median_and_previous_value();
		}
	}
	return m_median;
}

void MFHistogram::move_median_down() {
	unsigned char j=m_median-1;
	while ((j>0)&&(m_data[j]==0)) j--;
	m_num_values_below_median-=m_data[j];
	m_median=j;
}
void MFHistogram::move_median_up() {
	unsigned char j=m_median+1;
	while ((j<255)&&(m_data[j]==0)) j++;
	m_num_values_below_median+=m_data[m_median];
	m_median=j;	
}
unsigned char MFHistogram::average_of_median_and_previous_value() {
	unsigned char j=m_median-1;
	while ((j>0)&&(m_data[j]==0)) j--;
	return (unsigned char)((m_median+j)/2);
}


void median_filter(int M,int N,unsigned char *output,unsigned char *input,unsigned char *mask,int radius) {	
	MFHistogram Hist;
	
	if (M<radius+1) return;
	if (N<radius+1) return;

	//initialize at (0,0)
	for (int x0=0; x0<=radius; x0++)
		for (int y0=0; y0<=radius; y0++)
			if ((!mask)||(mask[x0+M*y0]))
				Hist.add(input[x0+M*y0]);	
	output[0+M*0]=Hist.median();
		
	for (int y=0; y<N; y++) {
		int x;
		if (y%2==0) { //move in positive direction
			for (x=1; x<M; x++) {
				if (x-radius-1>=0) { //remove left edge
					for (int dy=-radius; dy<=radius; dy++) {
						if ((0<=y+dy)&&(y+dy<N)) {
							if ((!mask)||(mask[x-radius-1+M*(y+dy)])) {
								Hist.remove(input[x-radius-1+M*(y+dy)]);
							}
						}
					}
				}
				if (x+radius<M) { //add right edge
					for (int dy=-radius; dy<=radius; dy++) {
						if ((0<=y+dy)&&(y+dy<N)) {
							if ((!mask)||(mask[x+radius+M*(y+dy)])) {
								Hist.add(input[x+radius+M*(y+dy)]);
							}
						}
					}
				}
				output[x+M*y]=Hist.median();
			}
			x=M-1;
		}
		else { //move in negative direction
			for (x=M-2; x>=0; x--) {
				if (x-radius>=0) { //add left edge
					for (int dy=-radius; dy<=radius; dy++) {
						if ((0<=y+dy)&&(y+dy<N)) {
							if ((!mask)||(mask[x-radius+M*(y+dy)])) {
								Hist.add(input[x-radius+M*(y+dy)]);
							}
						}
					}
				}
				if (x+radius+1<M) { //remove right edge
					for (int dy=-radius; dy<=radius; dy++) {
						if ((0<=y+dy)&&(y+dy<N)) {
							if ((!mask)||(mask[x+radius+1+M*(y+dy)])) {
								Hist.remove(input[x+radius+1+M*(y+dy)]);
							}
								
						}
					}
				}
				output[x+M*y]=Hist.median();
			}
			x=0;
		}
		if (y+1<N) { //move down one unit
			if (y+1-radius-1>=0) { //remove top edge
				for (int dx=-radius; dx<=radius; dx++) {
					if ((0<=x+dx)&&(x+dx<M)) {
						if ((!mask)||(mask[x+dx+M*(y+1-radius-1)])) {
							Hist.remove(input[x+dx+M*(y+1-radius-1)]);
						}
					}
				}
			}
			if (y+1+radius<N) { //add bottom edge
				for (int dx=-radius; dx<=radius; dx++) {
					if ((0<=x+dx)&&(x+dx<M)) {
						if ((!mask)||(mask[x+dx+M*(y+1+radius)])) {
							Hist.add(input[x+dx+M*(y+1+radius)]);
						}
					}
				}
			}
			output[x+M*(y+1)]=Hist.median();
		}
	}
}

QImage median_filter(const QImage &img,unsigned char *mask,int radius) {
	int W=img.width();
	int H=img.height();
	if (W*H==0) return QImage();
	QImage ret(W,H,QImage::Format_RGB32);
	unsigned char *input=(unsigned char *)malloc(sizeof(unsigned char)*W*H);
	unsigned char *output=(unsigned char *)malloc(sizeof(unsigned char)*W*H);
	for (int channel=0; channel<3; channel++) {
		for (int y=0; y<H; y++)
		for (int x=0; x<W; x++) {
			QColor col=img.pixel(x,y);
			if (channel==0) input[x+W*y]=col.red();
			else if (channel==1) input[x+W*y]=col.green();
			else if (channel==2) input[x+W*y]=col.blue();
		}	
		median_filter(W,H,output,input,mask,radius);
		for (int y=0; y<H; y++)
		for (int x=0; x<W; x++) {
			QColor col=ret.pixel(x,y);
			if (channel==0) col.setRed(output[x+W*y]);
			else if (channel==1) col.setGreen(output[x+W*y]);
			else if (channel==2) col.setBlue(output[x+W*y]);
			ret.setPixel(x,y,col.rgb());
		}	
	}
	free(input);
	free(output);
	return ret;
}

void median_filter_3d(int M,int N,int P,unsigned char *output,unsigned char *input,unsigned char *mask,int radius) {	
	MFHistogram Hist;
	
	if (M<radius+1) return;
	if (N<radius+1) return;
	if (P<radius+1) return;

	for (int z=0; z<P; z++) {
		int z0min=z-radius;
		int z0max=z+radius;
		if (z0min<0) z0min=0;
		if (z0max>=P) z0max=P-1;
		//initialize at (0,0,z)
		Hist.clear();
		for (int x0=0; x0<=radius; x0++)
		for (int y0=0; y0<=radius; y0++)
		for (int z0=z0min; z0<=z0max; z0++) {
			int zoffset=z0*M*N;
			if ((!mask)||(mask[x0+M*y0+zoffset]))
				Hist.add(input[x0+M*y0+zoffset]);	
		}
		output[0+M*0+z*M*N]=Hist.median();
			
			
		for (int y=0; y<N; y++) {
			int x;
			if (y%2==0) { //move in positive direction
				for (x=1; x<M; x++) {
					if (x-radius-1>=0) { //remove left edge
						for (int z0=z0min; z0<=z0max; z0++) {
							int zoffset=z0*M*N;
							for (int dy=-radius; dy<=radius; dy++) {
								if ((0<=y+dy)&&(y+dy<N)) {
									if ((!mask)||(mask[x-radius-1+M*(y+dy)+zoffset])) {
										Hist.remove(input[x-radius-1+M*(y+dy)+zoffset]);
									}
								}
							}
						}
					}
					if (x+radius<M) { //add right edge
						for (int z0=z0min; z0<=z0max; z0++) {
							int zoffset=z0*M*N;
							for (int dy=-radius; dy<=radius; dy++) {
								if ((0<=y+dy)&&(y+dy<N)) {
									if ((!mask)||(mask[x+radius+M*(y+dy)+zoffset])) {
										Hist.add(input[x+radius+M*(y+dy)+zoffset]);
									}
								}
							}
						}
					}
					output[x+M*y+z*M*N]=Hist.median();
				}
				x=M-1;
			}
			else { //move in negative direction
				for (x=M-2; x>=0; x--) {
					if (x-radius>=0) { //add left edge
						for (int z0=z0min; z0<=z0max; z0++) {
							int zoffset=z0*M*N;
							for (int dy=-radius; dy<=radius; dy++) {
								if ((0<=y+dy)&&(y+dy<N)) {
									if ((!mask)||(mask[x-radius+M*(y+dy)+zoffset])) {
										Hist.add(input[x-radius+M*(y+dy)+zoffset]);
									}
								}
							}
						}
					}
					if (x+radius+1<M) { //remove right edge
						for (int z0=z0min; z0<=z0max; z0++) {
							int zoffset=z0*M*N;
							for (int dy=-radius; dy<=radius; dy++) {
								if ((0<=y+dy)&&(y+dy<N)) {
									if ((!mask)||(mask[x+radius+1+M*(y+dy)+zoffset])) {
										Hist.remove(input[x+radius+1+M*(y+dy)+zoffset]);
									}
										
								}
							}
						}
					}
					output[x+M*y+z*M*N]=Hist.median();
				}
				x=0;
			}
			if (y+1<N) { //move down one unit
				if (y+1-radius-1>=0) { //remove top edge
					for (int z0=z0min; z0<=z0max; z0++) {
						int zoffset=z0*M*N;
						for (int dx=-radius; dx<=radius; dx++) {
							if ((0<=x+dx)&&(x+dx<M)) {
								if ((!mask)||(mask[x+dx+M*(y+1-radius-1)+zoffset])) {
									Hist.remove(input[x+dx+M*(y+1-radius-1)+zoffset]);
								}
							}
						}
					}
				}
				if (y+1+radius<N) { //add bottom edge
					for (int z0=z0min; z0<=z0max; z0++) {
						int zoffset=z0*M*N;
						for (int dx=-radius; dx<=radius; dx++) {
							if ((0<=x+dx)&&(x+dx<M)) {
								if ((!mask)||(mask[x+dx+M*(y+1+radius)+zoffset])) {
									Hist.add(input[x+dx+M*(y+1+radius)+zoffset]);
								}
							}
						}
					}
				}
				output[x+M*(y+1)+z*M*N]=Hist.median();
			}
		}
	}
}
