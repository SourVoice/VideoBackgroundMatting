#pragma once

#include <string>
//Qtͷ�ļ�
#include <QTimer>
#include <qthread.h>
#include <QMutex>
#include <QtWidgets>
#include <QImage>
#include <QtGui>

class BGProcess :public QThread
{
	Q_OBJECT
public:
	explicit BGProcess(QObject* parent = 0);
	virtual ~BGProcess();

	void Open(const QString& fileInput);

protected:
	void run();							//��дrunʵ�ֽ���

private:
	QString video_path;
	char* in_file;						//��Ƶ��ַ(�ļ�)

};
