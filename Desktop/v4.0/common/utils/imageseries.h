#ifndef imageseries_H
#define imageseries_H

#include <QImage>

class ImageSeriesPrivate;
class ImageSeries {
public:
	friend class ImageSeriesPrivate;
	ImageSeries();
	virtual ~ImageSeries();
	int imageCount() const;
	void addImage(const QImage &img);
	QImage image(int index) const;
	void clear();
	void setBitrate(int br);
	void setFramesPerSecond(int fps);
	void writeMovie(QString mp4_fname) const;
private:
	ImageSeriesPrivate *d;
};

#endif
