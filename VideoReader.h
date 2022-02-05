#pragma once
#include  "pch.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

#include <iostream>
#include <opencv2/opencv.hpp>

#define INBUF_SIZE 1920*1080


class VideoReader
{
public:
	VideoReader(std::string filename);
	~VideoReader();
	int init();
	void flush();
	int read(cv::Mat &frame);

private:
	cv::Mat AVFrame2Img(AVFrame *pFrame);

	AVFormatContext* ifmt_ctx = NULL;
	const AVCodec* codec = NULL;
	AVCodecContext* codec_ctx = NULL;

	AVPacket* pkt = NULL;
	AVFrame* frame = NULL;
	//bool is_finished = false;
	std::string in_filename;
};

