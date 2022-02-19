#pragma once
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

#include <iostream>
#include <opencv2/opencv.hpp>


class VideoReader
{
public:
	VideoReader(std::string filename);
	~VideoReader();
	int init();
	void flush();
	int read(cv::Mat &frame);
	int get_fps();
	double get_frame_cnt();
	int get_bit_rate();
	int get_width();
	int get_height();
	int get_video_stream_idx();

private:
	int AVFrame2Img(AVFrame *pFrame, cv::Mat &mat);

	AVFormatContext* ifmt_ctx = NULL;
	const AVCodec* codec = NULL;
	AVCodecContext* codec_ctx = NULL;

	AVPacket* pkt = NULL;
	AVFrame* frame = NULL;
	std::string in_filename;
	int video_stream_idx = -1;
	int frame_cnt;
	int fps;
	int bit_rate;
	int width;
	int height;
	double total_time_second;
};