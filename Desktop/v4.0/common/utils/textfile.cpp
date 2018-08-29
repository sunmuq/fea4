#include "textfile.h"
#include <QFile>
#include <QTextStream>

QString read_text_file(const QString & fname, QTextCodec *codec) {
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		return QString();
	QTextStream ts(&file);
	if (codec != 0)	
		ts.setCodec(codec);
	QString ret = ts.readAll();
	file.close();
	return ret;
}

bool write_text_file(const QString & fname,const QString &txt, QTextCodec *codec) {
	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
		return false;
	QTextStream ts(&file);
	if (codec != 0){
		ts.setAutoDetectUnicode(false);
		ts.setCodec(codec);
	}
	ts << txt;
	ts.flush();
	file.close();
	return true;
}


