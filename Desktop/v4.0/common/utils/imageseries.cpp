#include "imageseries.h"
#include <QList>
#include <QDir>
#include <QImageWriter>
#include <QDebug>
#include <QApplication>
#include <QProcess>

class ImageSeriesPrivate {
public:
	ImageSeries *q;
	QList<QImage> m_images;
	int m_bitrate;
	int m_frames_per_second;
};

ImageSeries::ImageSeries() 
{
	d=new ImageSeriesPrivate;
	d->q=this;
	d->m_bitrate=100;
	d->m_frames_per_second=5;
}

ImageSeries::~ImageSeries()
{
	delete d;
}

int ImageSeries::imageCount() const {
	return d->m_images.count();
}
void ImageSeries::addImage(const QImage &img) {
	d->m_images << img;
}
QImage ImageSeries::image(int index) const {
	return d->m_images[index];
}
void ImageSeries::clear() {
	d->m_images.clear();
}

void ImageSeries::writeMovie(QString mp4_fname) const {
	QFile::remove(mp4_fname);
	if (QFile(mp4_fname).exists()) {
		qWarning() << "Could not remove file: "+mp4_fname;
		return;
	}
	QString tmpname=QString("ImageSeries_%1").arg(qrand()%10000);
	QDir(QDir::tempPath()).mkdir(tmpname);
	QString dirname=QDir::tempPath()+"/"+tmpname;
	
	{
		QStringList list=QDir(dirname).entryList(QStringList("*.*"));
		foreach (QString str,list) {
			QFile::remove(dirname+"/"+str);
		}
	}
	
	for (int i=0; i<d->m_images.count(); i++) {
		QString fname=dirname+"/"+QString("%1.jpg").arg(i,4,10,QChar('0'));
		QImageWriter IW(fname);
		IW.write(d->m_images[i]);
	}
	
	#ifdef WIN32
	mp4_fname.replace("/","\\");
	#endif
	
	
	
	
	{ //create the .avi
		QStringList args;
		QString tmp1=QString("w=%1:h=%2:fps=%3:type=jpg").arg(d->m_images[0].width()).arg(d->m_images[0].height()).arg(d->m_frames_per_second);
		args << "mf://*.jpg" 
			<< "-mf" << tmp1 
			<< "-ovc" << "x264"
			<< "-x264encopts" << QString("bitrate=%1").arg(d->m_bitrate)
			<< "-o" << "tmp.avi";
		#ifdef WIN32
		QString exec=QString(getenv("DEVCOMMON"))+"/utils"+"/mencoder.exe";
		#else
		QString exec="mencoder";
		#endif
		QProcess process;
		process.setWorkingDirectory(dirname);
		process.start(exec,args);
		if (process.waitForStarted(1000)) {
			printf("Encoding movie: %d frames, fps=%d, bitrate=%d...\n",imageCount(),d->m_frames_per_second,d->m_bitrate);
			process.waitForFinished(-1);
		}
		else {
			qDebug() << process.readAll();
			qWarning() << "Error running: "+exec;			
			return;
		}
	}
	QString fname1=dirname+"/tmp.avi";
	QString fname2=dirname+"/tmp.h264";
	#ifdef WIN32
	fname1.replace("/","\\");
	fname2.replace("/","\\");
	#endif
	{ //dump the .h264
		QStringList args;
		args << fname1
			<< "-dumpvideo"
			<< "-dumpfile" << fname2;
		#ifdef WIN32
		QString exec=QString(getenv("DEVCOMMON"))+"/utils"+"/mplayer.exe";
		#else
		QString exec="mplayer";
		#endif
		QProcess process;
		process.setWorkingDirectory(QString(getenv("DEVCOMMON"))+"/utils");
		process.start(exec,args);
		if (process.waitForStarted(1000)) {
			printf("Dumping .h264.\n");
			process.waitForFinished(-1);
		}
		else {
			qDebug() << process.readAll();
			qWarning() << "Error running: "+exec;
			return;
		}
	}
	{ //create the .mp4
		QStringList args;
		args << QString("-create=%1").arg(fname2)
			<< QString("-rate=%1").arg(d->m_frames_per_second)
			<< mp4_fname;
		#ifdef WIN32
		QString exec=QString(getenv("DEVCOMMON"))+"/utils"+"/mp4creator.exe";
		#else
		QString exec="mp4creator";
		#endif
		QProcess process;
		process.setWorkingDirectory(QString(getenv("DEVCOMMON"))+"/utils");
		process.start(exec,args);
		if (process.waitForStarted(1000)) {
			printf("Creating mp4...\n");
			process.waitForFinished(-1);
			printf("Done.\n");
		}
		else {
			qDebug() << process.readAll();
			qWarning() << "Error running: "+exec;
			return;
		}
	}
	
	{
		QStringList list=QDir(dirname).entryList(QStringList("*.*"));
		foreach (QString str,list) {
			QFile::remove(dirname+"/"+str);
		}
	}
	QDir(QDir::tempPath()).rmdir(tmpname);
}

void ImageSeries::setFramesPerSecond(int fps) {
	d->m_frames_per_second=fps;
}
void ImageSeries::setBitrate(int br) {
	d->m_bitrate=br;
}
