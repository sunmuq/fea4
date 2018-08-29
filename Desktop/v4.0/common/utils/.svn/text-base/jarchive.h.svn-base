#ifndef jarchive_H
#define jarchive_H

#define JARCHIVE_MODE_READ 1
#define JARCHIVE_MODE_WRITE 2
#define JARCHIVE_MODE_APPEND 3

#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QStringList>

class JArchivePrivate;
class JArchive {
public:
	friend class JArchivePrivate;
	JArchive(const QString &fname);
	virtual ~JArchive();
	bool open(int open_mode);
	void close();
	bool writeData(const QString &key,const QByteArray &data,bool compress=true);
	QByteArray readData(const QString &key);
	bool writeValue(const QString &key,const QVariant &value);
	QVariant readValue(const QString &key);
	void setValue(const QString &key,const QVariant &value); //same as writeValue
	QVariant value(const QString &key); //same as readValue
	QStringList allKeys() const;
	void beginGroup(const QString &prefix);
	void endGroup();
	int beginReadArray(const QString &prefix);
	void beginWriteArray(const QString &prefix,int size);
	void setArrayIndex(int i);
	void endArray();
	bool addFile(const QString &key,const QString &filename,bool compress=true);
	bool extractFile(const QString &key,const QString &filename);	
	bool contains(const QString &key);
	
	void writeFloatArray(const QString &key,QList<float> &X,bool compress=true);
	void writeFloatArray(const QString &key,QVector<float> &X,bool compress=true);
	QList<float> readFloatArray(const QString &key);
	void writeFloatArray(const QString &key,float *X,qint32 N,bool compress=true);
	void readFloatArray(const QString &key,float *X,qint32 N);
	void writeDoubleArray(const QString &key,QList<double> &X,bool compress=true);
	void writeDoubleArray(const QString &key,double *X,qint32 N,bool compress=true);
	QList<double> readDoubleArray(const QString &key);
	void readDoubleArray(const QString &key,double *X,qint32 N);
	QStringList readStringList(const QString &key);
	void writeStringList(const QString &key,const QStringList &X,bool compress=true);
private:
	JArchivePrivate *d;
};



#endif
