#include "readabledicom.h"
#include <QHash>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#ifdef USING_DCMTK
#include "dcmtk/dcmimgle/dcmimage.h"
#endif

class ReadableDicomPrivate {
public:
	QHash<QString,QVariant> m_parameters;
	quint16 *m_data;
	long m_data_size;
};

ReadableDicom::ReadableDicom() {
	d=new ReadableDicomPrivate;
	d->m_data=0;
	d->m_data_size=0;
}
ReadableDicom::~ReadableDicom() {
	clear();
	delete d;
}

#ifndef  USING_DCMTK
QString get_dicom2readable_executable() {
	QString ret=qApp->applicationDirPath()+"/dicom2readable";
	#ifdef WIN32
	ret+=".exe";
	#endif
	if (QFile(ret).exists()) return ret;
	
	ret="dicom2readable";
	#ifdef WIN32
	ret+=".exe";
	#endif
	return ret;
}
#endif
bool ReadableDicom::readDicomFile(const QString &dicom_file) {
	clear();
	QString fname=dicom_file;
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	#ifdef USING_DCMTK		
		
		bool ret=false;
		DicomImage *di = new DicomImage(fname.toAscii().data());
		long S=di->getOutputDataSize(16);
		if (S==di->getWidth()*di->getHeight()*2) {
			quint16 *tmpdata=(quint16 *)di->getOutputData(16);
			if (tmpdata) {
				int w=(int)di->getWidth();
				int h=(int)di->getHeight();
				d->m_parameters["width"]=w;
				d->m_parameters["height"]=h;
				d->m_data_size=w*h;
				d->m_data=(quint16 *)malloc(sizeof(quint16)*d->m_data_size);
				for (long ii=0; ii<d->m_data_size; ii++) {
					d->m_data[ii]=tmpdata[ii];
				}
				ret=true;
			}
		}
		delete di;		
		return ret;
	#else
		QString dicom2readable_executable=get_dicom2readable_executable();
		if (dicom2readable_executable.isEmpty()) {
			qWarning() << "dicom2readable_executable is empty.";
			return false;
		}
		QString tmpfname=QDir::tempPath()+"/"+QFileInfo(fname).fileName();
		#ifdef WIN32
		tmpfname.replace("/","\\");
		#endif
		QProcess process;
		QStringList args;
		args << fname << tmpfname;
		process.start(dicom2readable_executable,args);
		if (!process.waitForStarted()) {
			qWarning() << "Could not start process:" << dicom2readable_executable;
			return false;
		}
		if (!process.waitForFinished()) {
			qWarning() << "Process did not finish:" << dicom2readable_executable << args;
			return false;
		}
		if (!QFile(tmpfname).exists()) {
			qWarning() << "Temporary file does not exist:" << tmpfname;
			return false;
		}
		bool ret=readReadableDicomFile(tmpfname);
		if (!ret) {
			qWarning() << "Error reading ReadableDicom file.";
		}
		QFile::remove(tmpfname);
		return ret;
	#endif
}
QString read_next_line(FILE *inf) {
	QString ret;
	bool done=false;
	char last_char=' ';
	while ((!feof(inf))&&(!done)) {
		char c;
		if (fread(&c,1,1,inf)>0) {
			ret.append(QChar(c));
			if ((c=='\n')&&(last_char=='\r')) {
				done=true;
			}
			last_char=c;
		}
		else done=true;
	}
	ret.remove('\n');
	ret.remove('\r');
	return ret;
}
bool ReadableDicom::readReadableDicomFile(const QString &readable_file) {
	clear();
	QString fname=readable_file;
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	FILE *inf=fopen(fname.toAscii().data(),"rb");
	if (inf) {
		bool done=false;
		while ((!feof(inf))&&(!done)) {
			QString line=read_next_line(inf);
			int ind=line.indexOf("=");
			if (ind>=0) {
				QString key=line.mid(0,ind);
				QString val=line.mid(ind+1);
				if (key=="data_size") {
					d->m_data_size=val.toLong();
					d->m_data=(quint16 *)malloc(sizeof(quint16)*d->m_data_size);
				}
				else if (key=="data") {
					for (long ii=0; ii<d->m_data_size; ii++) {
						if (fread(&d->m_data[ii],sizeof(quint16),1,inf)<=0) {
							qWarning() << "Error reading ReadableDicom file." << ii;
							break;
						}
					}
					done=true;
				}
				else {
					d->m_parameters[key]=val;
				}
			}
		}
		fclose(inf);
		return true;
	}
	else return false;
}
void write_string(FILE *f,QString str) {
	for (int i=0; i<str.count(); i++) {
		char c=str[i].toAscii();
		fwrite(&c,1,1,f);
	}
}
bool ReadableDicom::writeReadableDicomFile(const QString &readable_file) const {
	QString fname=readable_file;
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	FILE *f=fopen(fname.toAscii().data(),"wb");
	if (f) {
		write_string(f,QString("ReadableDicom Version=%1\r\n").arg(READABLE_DICOM_VERSION));
		QStringList keys=d->m_parameters.keys();
		foreach (QString key,keys) {
			QString str=QString("%1=%2\r\n").arg(key).arg(d->m_parameters[key].toString());
			write_string(f,str);
		}
		write_string(f,QString("data_size=%1\r\n").arg(d->m_data_size));
		write_string(f,QString("data=\r\n"));
		for (long ii=0; ii<d->m_data_size; ii++) {
			fwrite(&d->m_data[ii],sizeof(quint16),1,f);
		}
		fclose(f);
		return true;
	}
	else return false;
}
QVariant ReadableDicom::parameter(const QString &key) const {
	return d->m_parameters.value(key);
}
void ReadableDicom::setParameter(const QString &key,const QVariant &val) {
	d->m_parameters[key]=val;
}
int ReadableDicom::width() const {
	return d->m_parameters["width"].toInt();
}
int ReadableDicom::height() const {
	return d->m_parameters["height"].toInt();
}
quint16 ReadableDicom::data(int i,int j) const {
	if (!d->m_data) return 0;
	long ii=i+width()*j;
	if ((ii<0)||(ii>=d->m_data_size)) return 0;
	return d->m_data[ii];
}
void ReadableDicom::clear() {
	if (d->m_data) free(d->m_data);
	d->m_data=0;
	d->m_data_size=0;
	d->m_parameters.clear();
}
QStringList ReadableDicom::parameterKeys() const {
	return d->m_parameters.keys();
}
