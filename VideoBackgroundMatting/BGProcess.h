#pragma once
#include <string>
//Qtͷ�ļ�
#include <QTimer>
#include <qthread.h>
#include <QMutex>
#include <QtWidgets>
#include <QImage>
#include <QtGui>
#include <QString>
#include <iostream>
#include "VideoWriter.h"
#include "VideoReader.h"
#include "BackgroundMatting.h"


using namespace std;

class BGProcess :public QThread
{
	Q_OBJECT
public:
	explicit BGProcess(string in_file, string out_file, QObject* parent = 0);
	virtual ~BGProcess();

signals:
	void loading_modules();
	void loading_finished();
	void processed_frame_cnt(const int percent);
	void send_log(const QString msg);

protected:
	void run();							//��дrunʵ�ֽ���

private:
	string in_file;						//��Ƶ��ַ(�ļ�)
	string out_file;
	int frame_cnt;
	double total_frame_cnt;
};
