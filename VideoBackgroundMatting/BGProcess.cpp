#include "BGProcess.h"

#define TIMEMS      qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))

BGProcess::BGProcess(string in_file, string out_file, QObject* parent) :QThread(parent)
{
	this->in_file = in_file;
	this->out_file = out_file;
	frame_cnt = 0;
}
BGProcess::~BGProcess()
{
}


void BGProcess::run()
{
	
	while (1) {


		frame_cnt++;
		emit processed_frame_cnt(frame_cnt);
		if (frame_cnt >= 99) {
			break;
		}
		QThread::msleep(20);
	}
}
