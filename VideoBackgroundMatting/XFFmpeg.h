#ifndef XFFMPEG_H
#define XFFMPEG_H
#pragma execution_character_set("utf-8")
#pragma once

#include <iostream>
#include <string>
//����FFMpeg��ͷ�ļ�
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
//Qtͷ�ļ�
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

	static void initalizeLib();								//��ʼע��
	bool init();
	void Open(const QString& fileInput);					//����Ƶ�ļ�
	cv::Mat Avframe2cvMat(AVFrame* avframe, int w, int h);	//Avframe*תΪopenCV�е�Mat
	void releaseMem();										//�ͷŽ�����̲������ڴ�
	void Close();											//�ر��ļ�
	void Stop();
	void Play();
	std::string GetError();									//��ȡ������Ϣ

	static double AVRationalr2Double(AVRational r);			//����avrational�õ�ʱ��
	double getFrameRate(void);								//��ȡÿ��֡��

signals:
	void receiveImage(const QImage&image);					//�յ�ͼƬ�ź�
	void receiveTotalVideoTime(const double& time);
	void receiveCurrentVideoTime(const double& time);

public slots:
	void onGetIsPlay(const bool& isPlay);

protected:
	void run();							//��дrunʵ�ֽ���

private:
	char errorbuff[1024];				//��ʱ�����Ĵ�����Ϣ
	QMutex mutex;						//������������߳�ʱ����ͬʱ��Ķ�д
	QTime time;

	uint8_t* buffer;					//�洢�����ͼƬbuffer
	AVPacket* avPacket;					//������, ���ڴ洢����ǰ������
	AVStream* stream;					//
	AVFrame* avFrame;					//֡����, ���������
	AVFormatContext* avFormatContext;	//��ʽ����(���װ������)
	AVCodecContext* videoCodecCtx;		//��Ƶ������������
	AVCodecContext* audioCodec;			//��Ƶ������������(��Ƶδʵ��)
	SwsContext* swsContext;				//����ͼƬ���ݶ���

	AVDictionary* Options;				//��������
	AVCodec* videoCodec;				//��Ƶ������
	AVCodec* audioDecoder;				//��Ƶ������
	//QImage image;
	

public:
	QString path;
	char *in_file;						//��Ƶ��ַ(�ļ�)
	volatile bool Abort = false;		//�߳�ֹͣ��־λ
	volatile bool isPlay = true;		//������Ƶ��־λ
	int stream_index = -1;				//������Ϣ( 0 for video and 1 for audio)
	int videoStreamIndex = -1;			//������Ϣ( 0 for video and 1 for audio)
	int audioStreamIndex = -1;			//������Ϣ( 0 for video and 1 for audio)
	int videoWidth = 0;					//videoCodecCtx->width;
	int videoHeight = 0;				//videoCodecCtx->height;
	
	double frame_rate = 0;				//ÿ��֡��(fps)
	double currentVideoTime = 0;		//��ǰ֡(ʱ��)
	double totalVideoTime = 0;			//��ʱ��
};
#endif