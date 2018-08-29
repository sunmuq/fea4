#ifndef csvfile_H
#define csvfile_H

#include <QString>
#include <QStringList>

struct CsvFileLine {
	QStringList values;
};

class CsvFile {
public:
	CsvFile(QString fname,QChar delim=',');
	virtual ~CsvFile();
	void defineFromText(const QString &txt);
	int lineCount() {return m_lines.count();}
	int columnCount() {if (m_lines.count()==0) return 0; return m_lines[0].values.count();}
	QString value(int line,int column) {return m_lines.value(line).values.value(column);}
	QString value(int line,QString header);
	int columnIndex(QString header);
private:
	QChar m_delim;
	QList<CsvFileLine> m_lines;

};

#endif
