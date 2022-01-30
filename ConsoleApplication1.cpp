

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "BackgroundMatting.h"
#include <net.h>

int main()
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