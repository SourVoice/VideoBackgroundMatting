#pragma once

#include <string>
//Qt头文件
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
	void run();							//覆写run实现进程

private:
	QString video_path;
	char* in_file;						//视频地址(文件)

};
