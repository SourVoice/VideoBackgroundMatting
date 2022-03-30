#include "VideoReader.h"

VideoReader::VideoReader(std::string filename)
{
	in_filename = filename;
	frame_cnt = -1;
	fps = -1;
	width = 0;
	height = 0;
	bit_rate = 0;
	init();
}


VideoReader::~VideoReader()
{

	av_frame_free(&frame);
	av_packet_free(&pkt);
	avcodec_free_context(&codec_ctx);

	frame = nullptr;
	pkt = nullptr;
	codec_ctx = nullptr;
	codec = nullptr;
	ifmt_ctx = nullptr;
}

int VideoReader::init()
{
	int ret = 0;

	ifmt_ctx = NULL;
	codec = NULL;
	codec_ctx = NULL;

	pkt = NULL;
	frame = NULL;

	pkt = av_packet_alloc();

	frame = av_frame_alloc();

	// 1、打开多媒体文件
	ret = avformat_open_input(&ifmt_ctx, in_filename.c_str(), NULL, NULL);
	if (ret < 0)
	{
		printf("decoder cannot open the video %s\n", in_filename.c_str());
		return -1;
	}

	ret = avformat_find_stream_info(ifmt_ctx, NULL);
	if (ret < 0)
	{
		std::cout << "reader avformat_find_stream_info error,ret=" << ret << std::endl;
		return -5;
	}
	av_dump_format(ifmt_ctx, 0, in_filename.data(), 0);
	total_time_second = ifmt_ctx->duration / (AV_TIME_BASE);

	for (size_t i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		if (ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			//avcodec_parameters_to_context(codec_ctx, ifmt_ctx->streams[i]->codecpar);
			video_stream_idx = i;
			AVStream *in_stream = ifmt_ctx->streams[i];
			if (in_stream->avg_frame_rate.den != 0 && in_stream->avg_frame_rate.num != 0)
			{
				fps = in_stream->avg_frame_rate.num / in_stream->avg_frame_rate.den;//每秒多少帧 
			}

			frame_cnt = in_stream->nb_frames; //视频帧数
			width = in_stream->codecpar->width;
			height = in_stream->codecpar->height;
			bit_rate = in_stream->codecpar->bit_rate;
			
			 
			break;
		}
	}
	if (video_stream_idx == -1) {
		std::cout << "no video stream found" << std::endl;
		return -1;
	}
	// 2、find解码器

	codec = (AVCodec*)avcodec_find_decoder(ifmt_ctx->streams[video_stream_idx]->codecpar->codec_id);
	if (codec == NULL) {
		std::cout << "no decoder available" << std::endl;
		return -1;
	}

	// 3、分配解码器上下文
	codec_ctx = avcodec_alloc_context3(codec);
	if (codec_ctx == NULL)
	{
		std::cout << "reader avcodec_alloc_context3 error" << std::endl;
	}

	// 4、获取解码参数，支持mp4/H264文件,这几行代码非常重要，否则不能解码mp4到yuv
	//设置加速解码
	codec_ctx->lowres = codec->max_lowres;
	codec_ctx->flags2 |= AV_CODEC_FLAG2_FAST;
	avcodec_parameters_to_context(codec_ctx, ifmt_ctx->streams[video_stream_idx]->codecpar);

	
	int thread_count = std::thread::hardware_concurrency();
	codec_ctx->thread_count = thread_count > 0 ? thread_count : 4;
	printf("Video decoder opens %d threads\n", codec_ctx->thread_count);

	// 5、open解码器
	ret = avcodec_open2(codec_ctx, codec, NULL);
	if (ret < 0)
	{
		std::cout << "reader avcodec_open2 error, ret=" << ret << std::endl;
		return -1;
	}
}

void VideoReader::flush()
{
	avcodec_free_context(&codec_ctx);
	avformat_free_context(ifmt_ctx);

	av_packet_free(&pkt);
	av_frame_free(&frame);
}

int VideoReader::read(cv::Mat &img)
{
	int ret;
	if (av_read_frame(ifmt_ctx, pkt) >= 0)
	{
		// 如果是视频
		if (ifmt_ctx->streams[pkt->stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			if (pkt->size)
			{
				ret = avcodec_send_packet(codec_ctx, pkt);
				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					//std::cout << "avcodec_send_packet: " << ret << std::endl;
					return 0;
				}
				ret = avcodec_receive_frame(codec_ctx, frame);
				if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					//std::cout << "avcodec_receive_packet: " << ret << std::endl;
					return 0;
				}
				else
				{
					ret = AVFrame2Img(frame, img);
					av_packet_unref(pkt);
					return 1;
				}
			}
			av_packet_unref(pkt);
		}
		else
		{
			return 0;
		}
	}
	return -1;
}

int VideoReader::get_fps()
{
	if (fps > 0) return fps;
	return -1;
}

double VideoReader::get_frame_cnt()
{
	return (double)(total_time_second * fps);
}

int VideoReader::get_bit_rate()
{
	return bit_rate;
}

int VideoReader::get_width()
{
	return width;
}

int VideoReader::get_height()
{
	return height;
}



int VideoReader::get_video_stream_idx()
{
	return video_stream_idx;
}


int VideoReader::AVFrame2Img(AVFrame * pFrame, cv::Mat &bgr)
{
	int frameHeight = height;
	int frameWidth = width;

	//创建保存yuv数据的buffer
	uchar* pDecodedBuffer = (uchar*)malloc(frameHeight*frameWidth * sizeof(uchar) * 3 / 2);

	//从AVFrame中获取yuv420p数据，并保存到buffer
	int i, j, k;
	//拷贝y分量
	for (i = 0; i < frameHeight; i++)
	{
		memcpy(pDecodedBuffer + frameWidth * i,
			pFrame->data[0] + pFrame->linesize[0] * i,
			frameWidth);
	}
	//拷贝u分量
	for (j = 0; j < frameHeight / 2; j++)
	{
		memcpy(pDecodedBuffer + frameWidth * i + frameWidth / 2 * j,
			pFrame->data[1] + pFrame->linesize[1] * j,
			frameWidth / 2);
	}
	//拷贝v分量
	for (k = 0; k < frameHeight / 2; k++)
	{
		memcpy(pDecodedBuffer + frameWidth * i + frameWidth / 2 * j + frameWidth / 2 * k,
			pFrame->data[2] + pFrame->linesize[2] * k,
			frameWidth / 2);
	}
	//输出图像分配内存
	cv::Mat mat = cv::Mat(frameHeight * 3 / 2, frameWidth, CV_8UC1, pDecodedBuffer);
	cv::cvtColor(mat, bgr, cv::COLOR_YUV420p2BGR);
	//释放buffer
	free(pDecodedBuffer);
	return 1;
}


int main2s()
{
	VideoReader reader = VideoReader("test.flv");
	std::cout << reader.get_frame_cnt() << " "  << reader.get_fps() << std::endl;
	cv::Mat frame;
	while (true)
	{
		int ret = reader.read(frame);
		if(ret == 1)
		{
			cv::imshow("", frame);
			cv::waitKey(1);
		}
		else if (ret < 0)
		{
			break;
		}
		
	}
	return 1;
}