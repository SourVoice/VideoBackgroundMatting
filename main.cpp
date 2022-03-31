

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "BackgroundMatting.h"
#include <net.h>
#include "VideoWriter.h"
#include "VideoReader.h"

#include <ctime>

#pragma comment(lib, "./lib/avcodec.lib")
#pragma comment(lib, "./lib/avutil.lib")
#pragma comment(lib, "./lib/avformat.lib")
#pragma comment(lib, "./lib/swscale.lib")

//void test_image()
//{
//	BackgroundMatting bgm;
//	bgm.load();
//	cv::Mat img = cv::imread("E:/000000000785.jpg");
//	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
//	cv::Mat alpha;
//
//	auto start = std::chrono::high_resolution_clock::now();
//
//	bgm.draw(img, alpha);
//
//	auto end = std::chrono::high_resolution_clock::now();
//	float duration = std::chrono::duration<float, std::milli>(end - start).count();
//	std::cout << duration << std::endl;
//	//cv::imwrite("out.png", img);
//	std::cout << "success" << std::endl;
//	cv::imshow("res", img);
//	cv::imshow("mask", alpha);
//
//	cv::waitKey(0);
//}
//
//
//void test_opencv_videowriter()
//{
//	cv::VideoCapture capture("test.flv");
//	cv::VideoWriter writer;
//	
//	double fps = capture.get(cv::CAP_PROP_FPS);
//	writer.open("test_out_opendeopenenc.mp4", cv::VideoWriter::fourcc('m','p','4','v'), fps, cv::Size(1440, 1080), true);
//	
//	int i = 1;
//	cv::Mat frame;
//	double total = 0.0;
//	double start = clock();
//	while (i <= 1000)
//	{
//		capture >> frame;
//		writer.write(frame);
//		i++;
//		cv::waitKey(0);
//	}
//
//	double current = clock();
//	std::cout << (current - start) / i << "ms" << std::endl;
//	capture.release();
//	writer.release();
//}
//
//void test_videowriter()
//{
//	cv::VideoCapture capture("test.mp4");
//	double fps = capture.get(cv::CAP_PROP_FPS);
//	VideoWriter writer = VideoWriter(1440, 1080, "test_out_opendefenc.mp4", (int)fps, );
//	
//	int i = 1;
//	
//	double total = 0.0;
//	double start = clock();
//	while (i <= 1000)
//	{
//		//double start = clock();
//		cv::Mat frame;
//		capture >> frame;
//		writer.write(frame);
//		//double current = clock();
//		//total += (current - start);
//		//std::cout << total / i << "ms" << std::endl;
//		i++;
//	}
//	double current = clock();
//	std::cout << (current - start) / i << "ms" << std::endl;
//	writer.flush();
//	capture.release();
//}
//
//
//void test_video_matting_ffmpeg()
//{
//	cv::VideoCapture capture("test.flv");
//	double fps = capture.get(cv::CAP_PROP_FPS);
//	VideoWriter writer = VideoWriter(1440, 1080, "test_mat_ffmpeg.mp4", (int)fps);
//	BackgroundMatting bgm;
//	bgm.load();
//	cv::Mat alpha;
//	int i = 1;
//
//	double total = 0.0;
//	double start = clock();
//	while (i <= 200)
//	{
//		cv::Mat frame;
//		capture >> frame;
//		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//		bgm.draw(frame, alpha);
//		writer.write(frame);
//		
//		i++;
//	}
//	double current = clock();
//	std::cout << (current - start) / i << "ms" << std::endl;
//	writer.flush();
//	capture.release();
//}
//
//
//void test_opencv_matting()
//{
//	cv::VideoCapture capture("test.flv");
//	cv::VideoWriter writer;
//
//	double fps = capture.get(cv::CAP_PROP_FPS);
//	writer.open("test_matting_opencv.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(1440, 1080), true);
//
//	BackgroundMatting bgm;
//	bgm.load();
//	cv::Mat alpha;
//
//	int i = 1;
//	cv::Mat frame;
//	double total = 0.0;
//	double start = clock();
//	while (i <= 200)
//	{
//		capture >> frame;
//		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//		bgm.draw(frame, alpha);
//		cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
//		writer.write(frame);
//
//		i++;
//		cv::waitKey(0);
//	}
//
//	double current = clock();
//	std::cout << (current - start) / i << "ms" << std::endl;
//	capture.release();
//	writer.release();
//}



//#include <thread>
//#include <condition_variable>
//#include <list>
//#include <iomanip>
//using namespace std;
//
//std::mutex mtx;
//std::condition_variable oneFinished;
//
//const int MAX_QUEUE_LENGTH = 5;
//const int TOTAL_TEST_NUMBER = 300 * 6;
//std::queue<cv::Mat> arr;
//
//cv::VideoCapture capture("test.flv");
//double fps = capture.get(cv::CAP_PROP_FPS);
//VideoWriter writer = VideoWriter(1440, 1080, "test_mat_ffmpeg.mp4", (int)fps);
//BackgroundMatting t_bgm;
//cv::Mat alpha;
//
//int i = 1;
//
//
//void producer()
//{
//	while (true)
//	{
//		std::unique_lock<std::mutex> lock(mtx);
//		if (i >= TOTAL_TEST_NUMBER)
//		{
//			break;
//		}
//		oneFinished.wait(lock, []() {return arr.size() < MAX_QUEUE_LENGTH; });
//		cv::Mat frame;
//		capture >> frame;
//		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//		
//		arr.push(frame);
//		i++;
//		oneFinished.notify_all();
//	}
//}
//
//void consumer()
//{
//	while (true)
//	{
//		std::unique_lock<std::mutex> lock(mtx);
//		if (i >= TOTAL_TEST_NUMBER && arr.empty())
//		{
//			break;
//		}
//		oneFinished.wait(lock, []() { return arr.size() > 0; });
//		
//		cv::Mat frame = arr.front();
//		arr.pop();
//		t_bgm.draw(frame, alpha);
//		writer.write(frame);
//		
//		oneFinished.notify_all();
//	}
//}


void test_video_matting_ffmpeg_read_write()
{
	VideoReader reader = VideoReader("testtest.flv");
	int fps = reader.get_fps();
	int height = reader.get_height();
	int width = reader.get_width();
	int bit_rate = reader.get_bit_rate();

	VideoWriter writer = VideoWriter(width, height, "test.mp4", fps, bit_rate);
	
	//BackgroundMatting bgm;
	cv::Mat frame;
	cv::Mat alpha;
	int i = 1;
	double total = 0.0;
	double start = clock();
	int ret = 1;
	while (true)
	{
		int ret = reader.read(frame);
		if (ret == 1)
		{
			
			//cv::imshow("", frame);
			//cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
			//std::cout << frame.rows << " " << frame.cols << std::endl;
			writer.write(frame);
			//cv::waitKey(1);
			i++;
		}
		else if (ret < 0)
		{
			break;
		}
		if (i >= 500) {
			break;
		}
	}
	double current = clock();
	std::cout << (current - start) / i << "ms" << std::endl;
	writer.flush();
	reader.flush();
}

int main(int argc, char *argv[])
{
	test_video_matting_ffmpeg_read_write();
	//test_video_read_ffmpeg();
	//test_video_matting_ffmpeg();

	//std::thread p(producer);
	//std::thread c(consumer);
	//double start = clock();
	//p.join();
	//c.join();
	//double current = clock();
	//
	//std::cout << (current - start) / i << "ms" << std::endl;
	//writer.flush();
	//capture.release();
	return 0;
}