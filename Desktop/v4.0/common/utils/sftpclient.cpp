#include "sftpclient.h"
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QTime>
#include <QFileInfo>
#include <QTest>

class SftpClientPrivate {
public:
	SftpClient *q;
	QProcess m_process;
	QStringList m_output_lines;
	QString m_executable_path;
	QString m_hostkey;
	int m_timeout; //seconds

	bool wait_for_prompt(int secs);	
	QString last_output_line();
	void clear_output();
};

SftpClient::SftpClient() 
{
	d=new SftpClientPrivate;
	d->q=this;
	d->m_timeout=60;
	connect(&d->m_process,SIGNAL(readyReadStandardError()),this,SLOT(slot_read_error()));
	connect(&d->m_process,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_read_output()));
}

SftpClient::~SftpClient()
{
	delete d;
}

bool SftpClient::open(QString host,QString user,QString passwd) {
	QString program=d->m_executable_path;
	if (program.isEmpty())
		program="c:\\program files (x86)\\winscp\\winscp.com";
	QStringList args; //args << user+"@"+host;
	d->m_process.start(program,args);
	while (d->m_process.state()==QProcess::Starting) {
		qApp->processEvents();
	}
	if (d->m_process.state()==QProcess::Running) {
		if (d->m_hostkey.isEmpty()) {
			d->m_process.write(QString("open %1@%2\n").arg(user).arg(host).toAscii());
		}
		else {
			d->m_process.write(QString("open %1@%2 -hostkey=\"%3\"\n").arg(user).arg(host).arg(d->m_hostkey).toAscii());
		}
		if (!d->wait_for_prompt(10)) return false;		
		d->m_process.write(QString(passwd+"\n").toAscii());
		if (!d->wait_for_prompt(10)) return false;		
		d->m_process.write(QString("option confirm off\n").toAscii());
		if (d->wait_for_prompt(10)) return true;
	}
	return false;
}
QStringList SftpClient::ls(QString remote_dirname) {
	d->clear_output();
	d->m_process.write(QString("ls "+remote_dirname+"\n").toAscii());
	if (d->wait_for_prompt(10)) {
		QStringList ret;
		foreach (QString str,d->m_output_lines) {
			int ind=str.indexOf(":");
			if (ind>=0) {
				QString fname=str.mid(ind).mid(12);
				fname=fname.trimmed();
				ret << fname;
			}
		}
		return ret;
	}
	else return QStringList();
}
void SftpClient::slot_read_error() {
	qDebug()  << "ERROR:" << d->m_process.readAllStandardError();
}
void SftpClient::slot_read_output() {
	QStringList list=QString(d->m_process.readAllStandardOutput()).split('\n');
	foreach (QString str,list) {
		qDebug()  << str;
		d->m_output_lines << str;
	}
}
QString SftpClientPrivate::last_output_line() {
	return m_output_lines.value(m_output_lines.count()-1);
}
void SftpClientPrivate::clear_output() {
	m_output_lines.clear();
}
bool SftpClientPrivate::wait_for_prompt(int secs) {
	QString prompt="winscp> ";
	QTime timer; timer.start();
	while ((last_output_line()!=prompt)&&(timer.elapsed()<secs*1000)) {
		qApp->processEvents();
	}
	if (last_output_line()==prompt)
		return true;
	else 
		return false;
}
bool SftpClient::get(QString remote_fname,QString local_fname) {
	#ifdef WIN32
	local_fname.replace("/","\\");
	#endif
	d->clear_output();
	d->m_process.write(QString("get "+remote_fname+" "+local_fname+"\n").toAscii());
	if (!d->wait_for_prompt(d->m_timeout)) return false;
	bool ok=true;
	foreach (QString line,d->m_output_lines) {
		if ((line.indexOf("Error ")==0)&&(line.indexOf(QFileInfo(remote_fname).fileName())!=0)) ok=false;
	}
	return ok;
}
bool SftpClient::put(QString local_fname,QString remote_fname) {
	#ifdef WIN32
	local_fname.replace("/","\\");
	#endif
	d->clear_output();
	d->m_process.write(QString("put "+local_fname+" "+remote_fname+"\n").toAscii());
	if (!d->wait_for_prompt(d->m_timeout)) return false;
	bool ok=true;
	foreach (QString line,d->m_output_lines) {
		if ((line.indexOf("Error ")==0)&&(line.indexOf(QFileInfo(local_fname).fileName())!=0)) ok=false;
	}
	return ok;
}
bool SftpClient::close() {
	d->m_process.write("exit\n");
	return d->m_process.waitForFinished(3000);
}
void SftpClient::setExecutablePath(QString path) {
	d->m_executable_path=path;
}
void SftpClient::setHostKey(QString val) {
	d->m_hostkey=val;
}
void SftpClient::setTimeout(int secs) {
	d->m_timeout=secs;
}
