#include "pch.h"
#include "BackgroundMatting.h"


BackgroundMatting::BackgroundMatting()
{
	is_first = true;
	device_count = 8;
#if NCNN_VULKAN
	device_count = ncnn::get_gpu_count();
	net.opt.use_vulkan_compute = 1;
	net.opt.num_threads = device_count;
#endif
	net.opt.num_threads = device_count;
}


BackgroundMatting::~BackgroundMatting()
{
}

void BackgroundMatting::load(int model_size)
{
	target_size = model_size;
	if (model_size == 512) {
		net.load_param("rvm-512.param");
		net.load_model("rvm-512.bin");
	}
	else {
		std::cout << "load model failed !" << std::endl;
	}

	r1i = ncnn::Mat(128, 128, 16);
	r2i = ncnn::Mat(64, 64, 20);
	r3i = ncnn::Mat(32, 32, 40);
	r4i = ncnn::Mat(16, 16, 64);
	r1i.fill(0.0f);
	r2i.fill(0.0f);
	r3i.fill(0.0f);
	r4i.fill(0.0f);
}

void BackgroundMatting::process(cv::Mat img, cv::Mat &mask, cv::Mat &foreground)
{

	ncnn::Mat ncnn_in, ncnn_in1;

	ncnn_in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_RGB, img.cols, img.rows, 512, 512);
	ncnn_in1 = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_RGB, img.cols, img.rows, 256, 256);

	ncnn_in.substract_mean_normalize(means, norms);
	ncnn_in1.substract_mean_normalize(mean_vals, norm_vals);

	if (is_first == 0)
	{
		r1i = r1o.clone();
		r2i = r2o.clone();
		r3i = r3o.clone();
		r4i = r4o.clone();
	}

	ncnn::Extractor ex = net.create_extractor();
	ex.set_num_threads(device_count);

	ex.input("src1", ncnn_in1);
	ex.input("src2", ncnn_in);
	ex.input("r1i", r1i);
	ex.input("r2i", r2i);
	ex.input("r3i", r3i);
	ex.input("r4i", r4i);

	ncnn::Mat pha, fgr;
	ex.extract("r4o", r4o);
	ex.extract("r3o", r3o);
	ex.extract("r2o", r2o);
	ex.extract("r1o", r1o);
	ex.extract("pha", pha);
	ex.extract("fgr", fgr);

	float *pha_data = (float*)pha.data;
	float *fgr_data = (float*)fgr.data;

	cv::Mat cv_mask = cv::Mat::zeros(pha.h, pha.w, CV_8UC1);
	cv::Mat cv_pha = cv::Mat(pha.h, pha.w, CV_32FC1, pha_data);

	cv::Mat cv_fgr = cv::Mat::zeros(fgr.h, fgr.w, CV_8UC3);
	cv::Mat cv_foreground = cv::Mat(fgr.h, fgr.w, CV_32FC3);

	memcpy(cv_foreground.data, fgr_data, fgr.h * fgr.w * 3);
	/*if (target_size == 512)
	{
		for (int i = 0; i < 512; i++)
		{
			for (int j = 0; j < 512; j++)
			{
				cv_foreground.at<cv::Vec3f>(i, j)[0] = fgr_data[0 * 512 * 512 + i * 512 + j];
				cv_foreground.at<cv::Vec3f>(i, j)[1] = fgr_data[1 * 512 * 512 + i * 512 + j];
				cv_foreground.at<cv::Vec3f>(i, j)[2] = fgr_data[2 * 512 * 512 + i * 512 + j];
			}
		}
	}
	else {
		for (int i = 0; i < 640; i++)
		{
			for (int j = 0; j < 480; j++)
			{
				cv_foreground.at<cv::Vec3f>(i, j)[0] = fgr_data[0 * 480 * 640 + i * 480 + j];
				cv_foreground.at<cv::Vec3f>(i, j)[1] = fgr_data[1 * 480 * 640 + i * 480 + j];
				cv_foreground.at<cv::Vec3f>(i, j)[2] = fgr_data[2 * 480 * 640 + i * 480 + j];
			}
		}
	}*/
	
	cv_pha.convertTo(cv_mask, CV_8UC1, 255.0, 0);
	cv_foreground.convertTo(cv_fgr, CV_8UC3, 255.0, 0);

	cv_mask.copyTo(mask);
	cv_fgr.copyTo(foreground);
	is_first = 0;
}

int BackgroundMatting::draw(cv::Mat &rgb, cv::Mat &alpha)
{
	cv::Mat mask, fgr;
	process(rgb, mask, fgr);
	
	cv::resize(mask, alpha, rgb.size(), 0, 0, 1);
	cv::resize(fgr, fgr, rgb.size(), 0, 0, 1);

	cv::Mat dst;
	double thresh = cv::threshold(alpha, dst, 0, 255, cv::THRESH_OTSU);
	for (int i = 0; i < alpha.rows; i++)
	{
		for (int j = 0; j < alpha.cols; j++)
		{
			uchar data = alpha.at<uchar>(i, j);
			if (data < thresh) {
				rgb.at<cv::Vec3b>(i, j)[0] = 0;
				rgb.at<cv::Vec3b>(i, j)[1] = 0;
				rgb.at<cv::Vec3b>(i, j)[2] = 0;
			}
		}
	}
	
	return 0;
}
