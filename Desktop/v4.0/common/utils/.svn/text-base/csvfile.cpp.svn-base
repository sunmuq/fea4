#include "csvfile.h"
#include <QFile>
#include <QStringList>
#include <QByteArray>
#include <QDebug>

CsvFile::CsvFile(QString fname,QChar delim) 
{
	m_delim=delim;
	QFile f(fname);
	if (f.open(QIODevice::ReadOnly|QIODevice::Text)) {
		QByteArray all=f.readAll();
		QList<QByteArray> lines=all.split('\n');
		for (int j=0; j<lines.count(); j++) {
			QString line=QString(lines[j]);
			QList<QString> values;
			bool in_quote=false;
			QString tmpstr;
			for (int i=0; i<line.count(); i++) {
				QChar c=line[i];
				if (c=='\"') in_quote=!in_quote;
				else {
					if (in_quote) {
						tmpstr.append(c);
					}
					else {
						if (c==delim) {
							values << tmpstr;
							tmpstr="";
						}
						else {
							tmpstr.append(c);
						}
					}
				}
			}
			values << tmpstr;
			if (line.count()>0) {
				CsvFileLine L;
				for (int k=0; k<values.count(); k++) {
					QString val=values[k].trimmed();
					L.values << val;
				}
				m_lines << L;
			}
		}
	}	
}

void CsvFile::defineFromText(const QString &txt) {
	m_lines.clear();
	QStringList lines=txt.split("\n");
	for (int j=0; j<lines.count(); j++) {
		QStringList values=lines[j].split(m_delim);
		if (values.count()>0) {
			CsvFileLine L;
			for (int k=0; k<values.count(); k++)
				L.values << values[k].trimmed();
			m_lines << L;
		}
	}
}

CsvFile::~CsvFile()
{
}

int CsvFile::columnIndex(QString header) {
	if (m_lines.count()==0) return -1;
	for (int j=0; j<m_lines[0].values.count(); j++)
		if (m_lines[0].values[j]==header) return j;
	return -1;
}

QString CsvFile::value(int line,QString header) {
	int ind=columnIndex(header);
	if (ind>=0) return value(line,ind);
	else return "";
}
