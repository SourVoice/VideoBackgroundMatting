#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

using namespace std;

class VideoReader
{
public:
	VideoReader(string filename);
	~VideoReader();
	void init();
	void read();
	int read_a_frame(cv::Mat &mat);

private:
	void avframe2cvmat(AVFrame *avframe);
	char errorbuff[1024];
	string filename;
	uint8_t* buffer;					//�洢�����ͼƬbuffer
	AVPacket* avPacket;					//������, ���ڴ洢����ǰ������
	AVStream* stream;					//
	AVFrame* avFrame;					//֡����, ���������
	AVFormatContext* avFormatContext;	//��ʽ����(���װ������)
	AVCodecContext* videoCodecCtx;		//��Ƶ������������
	SwsContext* swsContext;				//����ͼƬ���ݶ���

	AVDictionary* Options;				//��������
	AVCodec* videoCodec;				//��Ƶ������
	int stream_index = -1;				//������Ϣ( 0 for video and 1 for audio)
	int videoStreamIndex = -1;			//������Ϣ( 0 for video and 1 for audio)

	int videoWidth = 0;					//videoCodecCtx->width;
	int videoHeight = 0;				//videoCodecCtx->height;
	double frame_rate = 0;				//ÿ��֡��(fps)
	double currentVideoTime = 0;		//��ǰ֡(ʱ��)
	double totalVideoTime = 0;			//��ʱ��
	int frame_idx = 0;
};

