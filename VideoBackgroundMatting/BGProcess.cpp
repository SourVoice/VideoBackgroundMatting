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
	emit this->loading_modules();
	BackgroundMatting bgm;
	bgm.load();
	int device_count = bgm.get_device_count();

	VideoReader reader = VideoReader(in_file);
	int fps = reader.get_fps();
	int height = reader.get_height();
	int width = reader.get_width();
	int bit_rate = reader.get_bit_rate();
	total_frame_cnt = reader.get_frame_cnt();

	MyVideoSpace::VideoWriter writer = MyVideoSpace::VideoWriter(width, height, out_file, fps, bit_rate, device_count);
	
	emit this->loading_finished();

	cv::Mat frame;
	cv::Mat alpha;
	
	double total = 0.0;
	double start = clock();
	int ret = 1;
	while (true)
	{
		int ret = reader.read(frame);
		if (ret == 1)
		{
			bgm.draw(frame, alpha);
			writer.write(frame);
			frame_cnt++;
			emit processed_frame_cnt((double)frame_cnt / total_frame_cnt * 100.0);
			if (frame_cnt % 100 == 0)
			{
				double current = clock();
				emit send_log(QString("Processing %1 frames costs %2 ms, %3 ms in average").arg(frame_cnt).arg(current - start, 0, 'g', 6).arg((current - start) / frame_cnt, 0, 'g', 4));
			}
		}
		else if (ret < 0)
		{
			break;
		}
		
	}

	double current = clock();
	emit send_log(QString("Finished!\nTotally %1 frames, %2 ms, %3 ms in average").arg(frame_cnt).arg(current - start, 0, 'g', 6).arg((current - start) / frame_cnt, 0, 'g', 4));

	writer.flush();
	reader.flush();
}
