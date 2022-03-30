#ifndef XFFMPEG_H
#define XFFMPEG_H
#pragma execution_character_set("utf-8")
#pragma once

#include <iostream>
#include <string>
//调用FFMpeg的头文件
extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavdevice/avdevice.h>
	#include <libavcodec/avcodec.h> 
	#include <libswresample/swresample.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/pixdesc.h>
	#include <libswscale/swscale.h>
	#include <libavformat/avio.h>
	#include <libavcodec/version.h>
	#include <libavutil/ffversion.h>
	#include <libavutil/rational.h>
}
//Qt头文件
#include <QTimer>
#include <qthread.h>
#include <QMutex>
#include <QtWidgets>
#include <QImage>
#include <QtGui>
//openCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>//for cvSmooth
using namespace cv;

class XFFmpeg : public QThread
{
	Q_OBJECT
public:
	explicit XFFmpeg(QObject* parent = 0);
	virtual ~XFFmpeg();

	static void initalizeLib();								//初始注册
	bool init();
	void Open(const QString& fileInput);					//打开视频文件
	cv::Mat Avframe2cvMat(AVFrame* avframe, int w, int h);	//Avframe*转为openCV中的Mat
	void releaseMem();										//释放解码过程产生的内存
	void Close();											//关闭文件
	void Stop();
	void Play();
	std::string GetError();									//获取错误信息

	static double AVRationalr2Double(AVRational r);			//计算avrational得到时间
	double getFrameRate(void);								//获取每秒帧数

signals:
	void receiveImage(const QImage&image);					//收到图片信号
	void receiveTotalVideoTime(const double& time);
	void receiveCurrentVideoTime(const double& time);

public slots:
	void onGetIsPlay(const bool& isPlay);

protected:
	void run();							//覆写run实现进程

private:
	char errorbuff[1024];				//打开时发生的错误信息
	QMutex mutex;						//互斥变量，多线程时避免同时间的读写
	QTime time;

	uint8_t* buffer;					//存储解码后图片buffer
	AVPacket* avPacket;					//包对象, 用于存储解码前的数据
	AVStream* stream;					//
	AVFrame* avFrame;					//帧对象, 解码后数据
	AVFormatContext* avFormatContext;	//格式对象(解封装上下文)
	AVCodecContext* videoCodecCtx;		//视频解码器上下文
	AVCodecContext* audioCodec;			//音频解码器上下文(音频未实现)
	SwsContext* swsContext;				//处理图片数据对象

	AVDictionary* Options;				//参数对象
	AVCodec* videoCodec;				//视频解码器
	AVCodec* audioDecoder;				//音频解码器
	//QImage image;
	

public:
	QString path;
	char *in_file;						//视频地址(文件)
	volatile bool Abort = false;		//线程停止标志位
	volatile bool isPlay = true;		//播放视频标志位
	int stream_index = -1;				//解码信息( 0 for video and 1 for audio)
	int videoStreamIndex = -1;			//解码信息( 0 for video and 1 for audio)
	int audioStreamIndex = -1;			//解码信息( 0 for video and 1 for audio)
	int videoWidth = 0;					//videoCodecCtx->width;
	int videoHeight = 0;				//videoCodecCtx->height;
	
	double frame_rate = 0;				//每秒帧数(fps)
	double currentVideoTime = 0;		//当前帧(时间)
	double totalVideoTime = 0;			//总时长
};
#endif