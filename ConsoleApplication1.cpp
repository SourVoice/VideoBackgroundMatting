﻿

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "BackgroundMatting.h"
#include <net.h>
#include "VideoWriter.h"

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

void test_videowriter()
{
	VideoWriter writer = VideoWriter(1440, 1080, "test_out.mp4");
	cv::VideoCapture capture("test.flv");
	int i = 0;
	while (i < 100)
	{
		cv::Mat frame;
		capture >> frame;
		writer.write(frame);
		cv::waitKey(25);
		
		i++;
	}
	writer.flush();
}

int main()
{
	test_videowriter();
	return 0;
}