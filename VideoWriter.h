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
}

class VideoWriter
{
public:
	VideoWriter(int w, int h, std::string file_out, int fps);
	~VideoWriter();

	int init();
	int write(cv::Mat bgr);
	int flush();

private:
	int fps = 30;
	int bit_rate = 2048 * 1024;
	int input_w, input_h;
	std::string filename_out;

	const AVCodec *pCodec;
	AVCodecContext *pCodecCtx = NULL;
	AVFormatContext* pFmtCtx;
	AVStream* vStream;
	AVFrame *pFrame;
	AVPacket *pkt;
	int frame_cnt;
	AVCodecID codec_id = AV_CODEC_ID_H264;
	uint8_t* y_buf;
	uint8_t* uv_buf;

	void rgb2nv12(cv::Mat bgr);
	
};

