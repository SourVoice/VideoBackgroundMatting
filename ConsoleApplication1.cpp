

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "BackgroundMatting.h"
#include <net.h>
#include "VideoWriter.h"
#include <ctime>

#pragma comment(lib, "./lib/avcodec.lib")
#pragma comment(lib, "./lib/avutil.lib")
#pragma comment(lib, "./lib/avformat.lib")

void test_image()
{
	BackgroundMatting bgm;
	bgm.load();
	cv::Mat img = cv::imread("E:/000000000785.jpg");
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	cv::Mat alpha;

	auto start = std::chrono::high_resolution_clock::now();

	bgm.draw(img, alpha);

	auto end = std::chrono::high_resolution_clock::now();
	float duration = std::chrono::duration<float, std::milli>(end - start).count();
	std::cout << duration << std::endl;
	//cv::imwrite("out.png", img);
	std::cout << "success" << std::endl;
	cv::imshow("res", img);
	cv::imshow("mask", alpha);

	cv::waitKey(0);
}


void test_opencv_videowriter()
{
	cv::VideoCapture capture("test.flv");
	cv::VideoWriter writer;
	
	double fps = capture.get(cv::CAP_PROP_FPS);
	writer.open("test_out_opendeopenenc.mp4", cv::VideoWriter::fourcc('m','p','4','v'), fps, cv::Size(1440, 1080), true);
	
	int i = 1;
	cv::Mat frame;
	double total = 0.0;
	double start = clock();
	while (i <= 1000)
	{
		capture >> frame;
		writer.write(frame);
		i++;
		cv::waitKey(0);
	}

	double current = clock();
	std::cout << (current - start) / i << "ms" << std::endl;
	capture.release();
	writer.release();
}

void test_videowriter()
{
	cv::VideoCapture capture("test.flv");
	double fps = capture.get(cv::CAP_PROP_FPS);
	VideoWriter writer = VideoWriter(1440, 1080, "test_out_opendefenc.mp4", (int)fps);
	
	int i = 1;
	
	double total = 0.0;
	double start = clock();
	while (i <= 1000)
	{
		//double start = clock();
		cv::Mat frame;
		capture >> frame;
		writer.write(frame);
		//double current = clock();
		//total += (current - start);
		//std::cout << total / i << "ms" << std::endl;
		i++;
	}
	double current = clock();
	std::cout << (current - start) / i << "ms" << std::endl;
	writer.flush();
	capture.release();
}


void test_video_matting_ffmpeg()
{
	cv::VideoCapture capture("test.flv");
	double fps = capture.get(cv::CAP_PROP_FPS);
	VideoWriter writer = VideoWriter(1440, 1080, "test_mat_ffmpeg.mp4", (int)fps);
	BackgroundMatting bgm;
	bgm.load();
	cv::Mat alpha;
	int i = 1;

	double total = 0.0;
	double start = clock();
	while (i <= 200)
	{
		cv::Mat frame;
		capture >> frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
		bgm.draw(frame, alpha);
		writer.write(frame);
		
		i++;
	}
	double current = clock();
	std::cout << (current - start) / i << "ms" << std::endl;
	writer.flush();
	capture.release();
}


void test_opencv_matting()
{
	cv::VideoCapture capture("test.flv");
	cv::VideoWriter writer;

	double fps = capture.get(cv::CAP_PROP_FPS);
	writer.open("test_matting_opencv.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(1440, 1080), true);

	BackgroundMatting bgm;
	bgm.load();
	cv::Mat alpha;

	int i = 1;
	cv::Mat frame;
	double total = 0.0;
	double start = clock();
	while (i <= 200)
	{
		capture >> frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
		bgm.draw(frame, alpha);
		cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
		writer.write(frame);

		i++;
		cv::waitKey(0);
	}

	double current = clock();
	std::cout << (current - start) / i << "ms" << std::endl;
	capture.release();
	writer.release();
}
int main()
{
	test_video_matting_ffmpeg();
	return 0;
}