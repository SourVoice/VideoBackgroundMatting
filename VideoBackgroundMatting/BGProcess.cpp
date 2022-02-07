#include "BGProcess.h"

#define TIMEMS      qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))

BGProcess::BGProcess(QObject* parent) :QThread(parent)
{
	video_path = nullptr;
}
BGProcess::~BGProcess()
{
}

void BGProcess::Open(const QString& fileInput)
{
	this->video_path = fileInput;
	std::string str = std::string(fileInput.toLocal8Bit());
	this->in_file = (char*)calloc(fileInput.size(), sizeof(char));
	strcpy(in_file, (char*)str.c_str());
}

void BGProcess::run()
{
	while (1) {
		qDebug() << TIMEMS << "BackgroundProcessing.";
	}
}
