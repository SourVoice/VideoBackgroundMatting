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
	uint8_t* buffer;					//存储解码后图片buffer
	AVPacket* avPacket;					//包对象, 用于存储解码前的数据
	AVStream* stream;					//
	AVFrame* avFrame;					//帧对象, 解码后数据
	AVFormatContext* avFormatContext;	//格式对象(解封装上下文)
	AVCodecContext* videoCodecCtx;		//视频解码器上下文
	SwsContext* swsContext;				//处理图片数据对象

	AVDictionary* Options;				//参数对象
	AVCodec* videoCodec;				//视频解码器
	int stream_index = -1;				//解码信息( 0 for video and 1 for audio)
	int videoStreamIndex = -1;			//解码信息( 0 for video and 1 for audio)

	int videoWidth = 0;					//videoCodecCtx->width;
	int videoHeight = 0;				//videoCodecCtx->height;
	double frame_rate = 0;				//每秒帧数(fps)
	double currentVideoTime = 0;		//当前帧(时间)
	double totalVideoTime = 0;			//总时长
	int frame_idx = 0;
};

