#ifndef sftpclient_H
#define sftpclient_H

#include <QStringList>
#include <QObject>

class SftpClientPrivate;
class SftpClient : public QObject {
	Q_OBJECT
public:
	friend class SftpClientPrivate;
	SftpClient();
	virtual ~SftpClient();
	void setExecutablePath(QString path); //path to WinSCP.com
	void setHostKey(QString val);
	void setTimeout(int secs);
	bool open(QString host,QString user,QString passwd);
	bool close();
	QStringList ls(QString remote_dirname);
	bool get(QString remote_fname,QString local_fname);
	bool put(QString local_fname,QString remote_fname);
private slots:
	void slot_read_error();
	void slot_read_output();
private:
	SftpClientPrivate *d;
};

#endif
