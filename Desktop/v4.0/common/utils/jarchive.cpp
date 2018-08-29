#include "jarchive.h"
#include <QFile>
#include <QHash>
#include <QDebug>
#include <QDataStream>

#define JARCHIVE_VERSION 1

#define JARCHIVE_MODE_CLOSED 0

#define JARCHIVE_BEGIN_FILE_CODE 21
#define JARCHIVE_BEGIN_RECORD_CODE 22


struct JArchiveRecordInfo {
	QString key;
	qint64 pos;
	qint32 length_of_data;
	qint32 compressed_length_of_data;
	bool is_compressed;
};

struct JArchivePrefix {
	QString name;
	int type; //1=group, 2=array, 3=array index
};

class JArchivePrivate {
public:
	JArchive *q;
	QFile m_file;
	QString m_file_name;
	int m_open_mode;
	QHash<QString,JArchiveRecordInfo> m_records;
	QString m_group_prefix;
	QList<JArchivePrefix> m_current_prefix_path;
	
	bool write_header();
	bool read_contents();
	
	void write_bytes(const QByteArray &X);
	void write_int32(qint32 x);
	QByteArray read_bytes(qint32 num);
	qint32 read_int32();
};

JArchive::JArchive(const QString &fname) 
{
	d=new JArchivePrivate;
	d->q=this;
	d->m_open_mode=JARCHIVE_MODE_CLOSED;
	d->m_file_name=fname;
}

JArchive::~JArchive()
{
	delete d;
}

bool JArchive::open(int open_mode) {
	d->m_file.setFileName(d->m_file_name);
	if (open_mode==JARCHIVE_MODE_WRITE) {
		if (!d->m_file.open(QIODevice::WriteOnly)) return false;
		d->write_header();
	}
	else if (open_mode==JARCHIVE_MODE_APPEND) {
		if (!d->m_file.open(QIODevice::WriteOnly|QIODevice::Append)) return false;
	}
	else if (open_mode==JARCHIVE_MODE_READ) {
		if (!d->m_file.open(QIODevice::ReadOnly)) return false;
		d->read_contents();
	}
	else return false;
	d->m_open_mode=open_mode;
	return true;
}
void JArchive::close() {	
	d->m_file.close();
	d->m_open_mode=JARCHIVE_MODE_CLOSED;
}
bool JArchivePrivate::write_header() {
	qint32 version_number=JARCHIVE_VERSION;
	qint32 header_size=4*3;
	qint32 code=JARCHIVE_BEGIN_FILE_CODE;
	
	write_int32(code); //4
	write_int32(header_size); //4
	write_int32(version_number); //4
	return true;
}
bool JArchivePrivate::read_contents() {
	m_records.clear();
	m_file.seek(0);
	qint32 header_size,version_number;
	{
		qint32 code=read_int32(); 
		if (code!=JARCHIVE_BEGIN_FILE_CODE) return false;
		header_size=read_int32();
		version_number=read_int32();
	}
	
	m_file.seek(header_size);
	qint64 filepos=m_file.pos();
	bool done=false;
	while ((!m_file.atEnd())&&(!done)) {
		qint32 code=read_int32();
		if (code==JARCHIVE_BEGIN_RECORD_CODE) {
			qint32 length_of_header,length_of_key,is_compressed;
			qint32 length_of_data,compressed_length_of_data;
			QByteArray key;
			length_of_header=read_int32(); //4
			compressed_length_of_data=read_int32(); //4
			length_of_data=read_int32(); //4
			length_of_key=read_int32(); //4
			key=read_bytes(length_of_key);
			is_compressed=read_int32(); //4
			JArchiveRecordInfo R;
			R.compressed_length_of_data=compressed_length_of_data;
			R.is_compressed=(is_compressed!=0);
			R.key=QString(key);
			R.length_of_data=length_of_data;
			R.pos=filepos+length_of_header;
			m_records[QString(key)]=R;
			m_file.seek(filepos+length_of_header+compressed_length_of_data);
			filepos=m_file.pos();
		}
		else {
		}
	}
	return true;
}
bool JArchive::writeData(const QString &key,const QByteArray &data,bool compress) {
	QByteArray data2;
	if (compress) data2=qCompress(data);
	
	QString tmp=key;
	if (!d->m_group_prefix.isEmpty())
		tmp=d->m_group_prefix+"/"+key;
	QByteArray key2=tmp.toAscii();
	qint32 length_of_header=4+4+4+4+key2.count()+4+4;
	qint32 length_of_data=data.count();
	qint32 compressed_length_of_data=length_of_data;
	if (compress) {
		compressed_length_of_data=data2.count();
	}
	qint32 length_of_key=key2.count();
	qint32 is_compressed=0;
	if (compress) is_compressed=1;
	qint32 code=JARCHIVE_BEGIN_RECORD_CODE;
	
	d->write_int32(code); //4
	d->write_int32(length_of_header); //4
	d->write_int32(compressed_length_of_data);	 //4
	d->write_int32(length_of_data); //4
	d->write_int32(length_of_key); //4
	d->write_bytes(key2); //length_of_key
	d->write_int32(is_compressed); //4
	if (compress) d->write_bytes(data2);
	else d->write_bytes(data);
	return true;
}
QByteArray JArchive::readData(const QString &key) {
	QString tmp=key;
	if (!d->m_group_prefix.isEmpty())
		tmp=d->m_group_prefix+"/"+key;
	QString key2=tmp;
	if (!d->m_records.contains(key2)) return "";
	JArchiveRecordInfo R=d->m_records[key2];
	d->m_file.seek(R.pos);
	QByteArray ret=d->m_file.read(R.compressed_length_of_data);
	if (R.is_compressed) {
		ret=qUncompress(ret);
	}
	return ret;
}
bool JArchive::contains(const QString &key) {
	QString tmp=key;
	if (!d->m_group_prefix.isEmpty())
		tmp=d->m_group_prefix+"/"+key;
	QString key2=tmp;
	return d->m_records.contains(key2);
}
bool JArchive::writeValue(const QString &key,const QVariant &value) {
	if (value.type()==QVariant::StringList) {
		QStringList list=value.toStringList();
		QString str="StringList::";
		for (int i=0; i<list.count(); i++) {
			str+=QString("{%1}").arg(list[i].count());
			str+=list[i];
		}
		return writeValue(key,str);
	}
	else {
		QByteArray tmp=value.toString().toAscii();
		return writeData(key,tmp,false);
	}
}
QVariant JArchive::readValue(const QString &key) {
	QVariant ret=QString(readData(key));
	
	QString str=ret.toString();
	if (str.mid(0,12)=="StringList::") {
		QStringList list;
		int i=12;
		while (i<str.count()) {
			int ind1=str.indexOf('{',i);
			int ind2=str.indexOf('}',i);
			if ((ind1>=0)&&(ind2>=0)) {
				int len=str.mid(ind1+1,ind2-ind1-1).toInt();
				list << str.mid(ind2+1,len);
				i=ind2+1+len;
			}
			else i=str.count();
		}
		return list;
	}
	else {	
		return ret;
	}
}
void JArchive::setValue(const QString &key,const QVariant &value) {
	writeValue(key,value);
}
QVariant JArchive::value(const QString &key) {
	return readValue(key);
}
bool JArchive::addFile(const QString &key,const QString &filename,bool compress) {
	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly)) return false;
	QByteArray tmp=f.readAll();
	f.close();
	return writeData(key,tmp);
}
bool JArchive::extractFile(const QString &key,const QString &filename) {
	QByteArray tmp=readData(key);
	QFile f(filename);
	if (!f.open(QIODevice::WriteOnly)) return false;
	f.write(tmp);
	f.close();
	return true;
}

void JArchivePrivate::write_int32(qint32 x) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	out << x;
	m_file.write(buf);
}
qint32 JArchivePrivate::read_int32() {
	qint32 ret;
	QByteArray buf=m_file.read(4);
	QDataStream in(buf);
	in.setVersion(QDataStream::Qt_4_5);
	in >> ret;
	return ret;
}
QByteArray JArchivePrivate::read_bytes(qint32 num) {
	return m_file.read(num);
}
void JArchivePrivate::write_bytes(const QByteArray &X) {
	m_file.write(X);
}
QStringList JArchive::allKeys() const {
	return d->m_records.keys();
}
void JArchive::writeFloatArray(const QString &key,QList<float> &X,bool compress) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 ct=X.count();
	out << ct;
	for (int i=0; i<ct; i++) {
		float val=X[i];
		out << val;
	}
	writeData(key,buf,compress);
}
void JArchive::writeFloatArray(const QString &key,QVector<float> &X,bool compress) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 ct=X.count();
	out << ct;
	for (int i=0; i<ct; i++) {
		float val=X[i];
		out << val;
	}
	writeData(key,buf,compress);
}
QList<float> JArchive::readFloatArray(const QString &key) {
	QByteArray buf=readData(key);
	QDataStream in(buf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 ct;
	in >> ct;
	QList<float> ret;
	for (int i=0; i<ct; i++) {
		float val;
		in >> val;
		ret << val;
	}
	return ret;
}
void JArchive::writeFloatArray(const QString &key,float *X,qint32 N,bool compress) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 ct=N;
	out << ct;
	for (int i=0; i<ct; i++) {
		float val=X[i];
		out << val;
	}
	writeData(key,buf,compress);
}
void JArchive::readFloatArray(const QString &key,float *X,qint32 N) {
	QByteArray buf=readData(key);
	QDataStream in(buf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 ct;
	in >> ct;
	if (ct>N) ct=N;
	QList<float> ret;
	for (int i=0; i<ct; i++) {
		float val;
		in >> val;
		X[i]=val;
	}
}
void JArchive::beginGroup(const QString &prefix) {
	JArchivePrefix P;
	P.name=prefix;
	P.type=1; //group
	d->m_current_prefix_path << P;
	if (d->m_group_prefix.isEmpty())
		d->m_group_prefix=prefix;
	else
		d->m_group_prefix+="/"+prefix;
}
void JArchive::endGroup() {
	if (d->m_current_prefix_path.count()>0) {
		int ii=d->m_current_prefix_path.count()-1;
		if (d->m_current_prefix_path[ii].type!=1) {
			qWarning() << "Unexpected endGroup for JArchive. **";
			return;		
		}
		d->m_current_prefix_path.removeAt(ii);
	}
	else {
		qWarning() << "Unexpected endGroup for JArchive.";
		return;
	};
	int ind=d->m_group_prefix.lastIndexOf('/');
	if (ind>=0) d->m_group_prefix=d->m_group_prefix.mid(0,ind);
	else d->m_group_prefix="";
}
int JArchive::beginReadArray(const QString &prefix) {
	JArchivePrefix P;
	P.name=prefix;
	P.type=2; //array
	d->m_current_prefix_path << P;
	if (d->m_group_prefix.isEmpty())
		d->m_group_prefix=prefix;
	else
		d->m_group_prefix+="/"+prefix;
		
	return readValue("size").toInt();
}
void JArchive::beginWriteArray(const QString &prefix,int size) {
	JArchivePrefix P;
	P.name=prefix;
	P.type=2; //array
	d->m_current_prefix_path << P;
	if (d->m_group_prefix.isEmpty())
		d->m_group_prefix=prefix;
	else
		d->m_group_prefix+="/"+prefix;
		
	writeValue("size",size);
}
void JArchive::setArrayIndex(int i) {
	{ //end array index if needed
		int ii=d->m_current_prefix_path.count()-1;
		if (ii>=0) {
			if (d->m_current_prefix_path[ii].type==3) { //array index
				d->m_current_prefix_path.removeAt(ii);
				int ind=d->m_group_prefix.lastIndexOf('/');
				if (ind>=0) d->m_group_prefix=d->m_group_prefix.mid(0,ind);
				else d->m_group_prefix="";
			}
		}
	}
	if (d->m_current_prefix_path.count()==0) {
		qWarning() << "Unexpected setArrayIndex for JArchive. **";
		return;
	}
	else {
		if (d->m_current_prefix_path[d->m_current_prefix_path.count()-1].type!=2) { //array
			qWarning() << "Unexpected setArrayIndex for JArchive.";
			return;			
		}
	}
	QString str=QString("%1").arg(i);
	JArchivePrefix P;
	P.name=str;
	P.type=3; //array index
	d->m_current_prefix_path << P;
	if (d->m_group_prefix.isEmpty())
		d->m_group_prefix=str;
	else
		d->m_group_prefix+="/"+str;
}
void JArchive::endArray() {
	{ //end array index if needed
		int ii=d->m_current_prefix_path.count()-1;
		if (ii>=0) {
			if (d->m_current_prefix_path[ii].type==3) { //array index
				d->m_current_prefix_path.removeAt(ii);
				int ind=d->m_group_prefix.lastIndexOf('/');
				if (ind>=0) d->m_group_prefix=d->m_group_prefix.mid(0,ind);
				else d->m_group_prefix="";
			}
		}
	}
	if (d->m_current_prefix_path.count()>0) {
		int ii=d->m_current_prefix_path.count()-1;
		if (d->m_current_prefix_path[ii].type!=2) {
			qWarning() << "Unexpected endArray for JArchive. **";
			return;		
		}
		d->m_current_prefix_path.removeAt(ii);
	}
	else {
		qWarning() << "Unexpected endArray for JArchive.";
		return;
	};
	int ind=d->m_group_prefix.lastIndexOf('/');
	if (ind>=0) d->m_group_prefix=d->m_group_prefix.mid(0,ind);
	else d->m_group_prefix="";
}
QStringList JArchive::readStringList(const QString &key) {
	QByteArray buf=readData(key);
	QDataStream in(buf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 ct;
	in >> ct;
	QStringList ret;
	for (int i=0; i<ct; i++) {
		qint32 size;
		in >> size;
		QString val;
		quint8 c;
		for (int j=0; j<size; j++) {
			in >> c;
			val.append(QChar(c));
		}
		ret << val;
	}
	return ret;
}
void JArchive::writeStringList(const QString &key,const QStringList &X,bool compress) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 ct=X.count();
	out << ct;
	for (int i=0; i<ct; i++) {
		qint32 size=X[i].count();
		out << size;
		for (int j=0; j<size; j++) {
			quint8 c=(X[i])[j].toAscii();
			out << c;
		}
	}
	writeData(key,buf,compress);
}
void JArchive::writeDoubleArray(const QString &key,QList<double> &X,bool compress) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 ct=X.count();
	out << ct;
	for (int i=0; i<ct; i++) {
		double val=X[i];
		out << val;
	}
	writeData(key,buf,compress);
}
QList<double> JArchive::readDoubleArray(const QString &key) {
	QByteArray buf=readData(key);
	QDataStream in(buf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 ct;
	in >> ct;
	QList<double> ret;
	for (int i=0; i<ct; i++) {
		double val;
		in >> val;
		ret << val;
	}
	return ret;
}
void JArchive::readDoubleArray(const QString &key,double *X,qint32 N) {
	QByteArray buf=readData(key);
	QDataStream in(buf);
	in.setVersion(QDataStream::Qt_4_5);
	qint32 ct;
	in >> ct;
	if (ct>N) ct=N;
	QList<float> ret;
	for (int i=0; i<ct; i++) {
		double val;
		in >> val;
		X[i]=val;
	}
}
void JArchive::writeDoubleArray(const QString &key,double *X,qint32 N,bool compress) {
	QByteArray buf;
	QDataStream out(&buf,QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	qint32 ct=N;
	out << ct;
	for (int i=0; i<ct; i++) {
		double val=X[i];
		out << val;
	}
	writeData(key,buf,compress);
}
