#ifndef readabledicom_H
#define readabledicom_H

#include <QString>
#include <QVariant>
class ReadableDicomPrivate;

#define READABLE_DICOM_VERSION "1.0"

/*
NOTE: 
If USING_DCMTK is defined, then readDicomFile() uses the dcmtk library to load the dicom image.
Otherwise, readDicomFile() uses the dicom2readable executable to load the image.
This way, the application that uses the ReadableDicom class does not need to link against the dcmtk library.
This is particularly important because it is not easy to get dcmtk to compile with MinGW on windows.
But I was able to get it to compile with QMAKESPEC=win32-mcvc2008.
So I created a statically linked version of dicom2readable.

You can either put the dicom2readable utility in a directory in your PATH, or put it in the same directory as your executable
file.
*/

class ReadableDicom {
public:
	friend class ReadableDicomPrivate;
	ReadableDicom();
	virtual ~ReadableDicom();
	bool readDicomFile(const QString &dicom_file);
	bool readReadableDicomFile(const QString &readable_file);
	bool writeReadableDicomFile(const QString &readable_file) const;
	QStringList parameterKeys() const;
	QVariant parameter(const QString &key) const;
	void setParameter(const QString &key,const QVariant &val);
	int width() const;
	int height() const;
	quint16 data(int i,int j) const;
	void clear();
private:
	ReadableDicomPrivate *d;
};

#endif
