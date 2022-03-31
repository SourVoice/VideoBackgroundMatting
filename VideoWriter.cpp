#include "pch.h"
#include "VideoWriter.h"


VideoWriter::VideoWriter(int w, int h, std::string file_out, int fps, int bit_rate)
{
	this->fps = fps;
	this->bit_rate = bit_rate;
	input_w = w;
	input_h = h;
	filename_out = file_out;
	frame_cnt = 0;
	init();
}


VideoWriter::~VideoWriter()
{
	free(y_buf);
	free(uv_buf);

	avcodec_free_context(&pCodecCtx);
	av_frame_free(&pFrame);
	pCodec = nullptr;
	pCodecCtx = nullptr;
	pFrame = nullptr;
}

int VideoWriter::init()
{
	if (avformat_alloc_output_context2(&pFmtCtx, nullptr, nullptr, filename_out.c_str()) < 0) {
		av_log(nullptr, AV_LOG_FATAL, "Could not allocate context.\n");
		return -1;
	}
	//Create and initialize a AVIOContext for accessing the resource indicated by url
	if (avio_open2(&pFmtCtx->pb, filename_out.c_str(), AVIO_FLAG_READ_WRITE, nullptr, nullptr) < 0) {
		av_log(nullptr, AV_LOG_FATAL, "Failed to open output file.\n");
		return -1;
	}
	//Add a new stream to a media file
	vStream = avformat_new_stream(pFmtCtx, nullptr);
	if (!vStream) {
		av_log(nullptr, AV_LOG_FATAL, "Failed to create stream channel.\n");
		return -1;
	}
	//Set codec parameter
	vStream->codecpar->width = input_w;
	vStream->codecpar->height = input_h;
	vStream->codecpar->bit_rate = bit_rate;
	vStream->codecpar->format = AV_PIX_FMT_NV12;
	vStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
	vStream->codecpar->codec_id = AV_CODEC_ID_H264;// AV_CODEC_ID_H264;
	//vStream->codecpar->sample_aspect_ratio = input_w >= input_h ? AVRational{ 4, 3 } : AVRational{3, 4};  // 这里设置4：3，视频就可以按照16：9显示
	
	pCodec = avcodec_find_encoder_by_name("h264_qsv");
	//pCodec = avcodec_find_encoder_by_name("libx264");

	if (!pCodec) {
		av_log(nullptr, AV_LOG_FATAL, "Can not find encoder.\n");
		return -1;
	}

	//Allocate an AVCodecContext and set its fields to default values
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (!pCodecCtx) {
		av_log(nullptr, AV_LOG_FATAL, "Could not allocate a codec context.\n");
		return -1;
	}
	//Fill the codec context based on the values from the supplied codec parameters
	if (avcodec_parameters_to_context(pCodecCtx, vStream->codecpar) < 0) {
		av_log(nullptr, AV_LOG_FATAL, "Codec not fill the codec context.\n");
		return -1;
	}
	//Set codec context parameter
	pCodecCtx->gop_size = 64;
	pCodecCtx->max_b_frames = 0;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = fps;
	pCodecCtx->bit_rate = bit_rate;
	pCodecCtx->thread_count = 4;
	
	//pCodecCtx->qmin = 10;
	//pCodecCtx->qmax = 30;

	//pCodecCtx->qcompress = 0.6;
	av_opt_set(pCodecCtx->priv_data, "preset", "veryfast", 0);
	//av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
	//av_opt_set(pCodecCtx->priv_data, "profile", "high", 0);

	//Open codec
	if (avcodec_open2(pCodecCtx, pCodec, nullptr) != 0) {
		av_log(nullptr, AV_LOG_FATAL, "Open encoder failure.\n");
		return -1;
	}

	//Allocate an AVPacketand set its fields to default values
	pkt = av_packet_alloc();
	if (!pkt) {
		av_log(nullptr, AV_LOG_FATAL, "Failed to initialize AVpacket.\n");
		return -1;
	}

	//sws_context = sws_getCachedContext(sws_context,
	//	input_w, input_h, AV_PIX_FMT_BGR24,    // 源格式
	//	input_w, input_h, AV_PIX_FMT_NV12,  // 目标格式
	//	SWS_BICUBIC,    // 尺寸变化使用算法
	//	0, 0, 0);

	//if (NULL == sws_context) {
	//	std::cout << "sws Context error" << std::endl;
	//	return -1;
	//}

	pFrame = av_frame_alloc();
	if (!pFrame) {
		av_log(nullptr, AV_LOG_FATAL, "Failed to allocate an AVFrame.\n");
		return -1;
	}

	pFrame->width = pCodecCtx->width;
	pFrame->height = pCodecCtx->height;
	pFrame->format = pCodecCtx->pix_fmt;
	
	//Allocate new buffer(s) for audio or video data
	if (av_frame_get_buffer(pFrame, 0) < 0) {
		av_log(nullptr, AV_LOG_FATAL, "Could not allocate the video frame data.\n");
		return -1;
	}
	//av_frame_make_writable(pFrame);

	y_step = pFrame->linesize[0];
	uv_step = pFrame->linesize[1];

	y_buf = (uint8_t *)malloc(y_step * input_h);
	uv_buf = (uint8_t *)malloc(uv_step * input_h);
	memset(y_buf, 0, y_step * input_h);
	memset(uv_buf, 0, uv_step * input_h);


	AVDictionary* opt = NULL;

	int frame_rate = fps;
	av_dict_set_int(&opt, "video_track_timescale", frame_rate, 0);

	//Allocate the stream private data and write the stream header to an output media file
	avformat_write_header(pFmtCtx, &opt);
	
	return 1;
}

int VideoWriter::write(cv::Mat rgb)
{
	rgb2nv12(rgb);

	//uint8_t *in_data[AV_NUM_DATA_POINTERS] = { 0 };
	//in_data[0] = rgb.data;
	//int in_size[AV_NUM_DATA_POINTERS] = { 0 };
	//// 一行（宽）数据的字节数
	//in_size[0] = rgb.cols * rgb.elemSize();
	//int h = sws_scale(sws_context, in_data, in_size, 0, rgb.rows,
	//	pFrame->data, pFrame->linesize);
	//if (h <= 0) { 
	//	av_log(nullptr, AV_LOG_FATAL, "sws scale failed.\n");
	//	return -1;
	//}

	pFrame->data[0] = y_buf;
	pFrame->data[1] = uv_buf;
	pFrame->pts = frame_cnt++;


	int res = avcodec_send_frame(pCodecCtx, pFrame);
	if (res < 0) {
		av_log(nullptr, AV_LOG_FATAL, "Error sending a frame for encoding.\n");
		return -1;
	}
	while (res >= 0) {
		//Read encoded data from the encoder
		res = avcodec_receive_packet(pCodecCtx, pkt);
		if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
			break;
		}
		if (res < 0) {
			av_log(nullptr, AV_LOG_FATAL, "Error during encoding.\n");
			return -1;
		}
		if (res >= 0) {
			pkt->stream_index = 0;
			//Write a packet to an output media file
			av_write_frame(pFmtCtx, pkt);
			av_packet_unref(pkt);
		}
	}

	//av_log(nullptr, AV_LOG_INFO, "frame pts: %d\n", i);
	return 0;
}



void VideoWriter::rgb2nv12(cv::Mat rgb)
{
	cv::Mat yuv_mat;
	cv::cvtColor(rgb, yuv_mat, cv::COLOR_RGB2YUV_I420);

	uint8_t *yuv = yuv_mat.ptr<uint8_t>();

	int32_t y_height = yuv_mat.rows * 2 / 3;
	int32_t y_width = yuv_mat.cols;

	// copy y data
	int32_t y_size_ = y_height * y_width;
	for (int i = 0; i < y_height; i++)
	{
		memcpy(y_buf + i * y_step, yuv + i * y_width, y_width);
	}
	int uv_height = y_height / 2;
	int u_size = y_size_ / 4;
	uint8_t *uv_p = uv_buf;
	uint8_t *u_data = yuv + y_size_;
	uint8_t *v_data = u_data + u_size;
	
	for (int32_t k = 0; k < uv_height; k++)
	{
		uv_p = uv_buf + k * uv_step;
		for (int32_t i = 0; i < y_width / 2; i++)
		{
			*uv_p++ = *u_data++;
			*uv_p++ = *v_data++;
		}
	}
	//
	//	for (int32_t j = 0; j < u_size; j++) {
	//		*uv_p++ = *u_data++;
	//		*uv_p++ = *v_data++;
	//	}
	//}
}

int VideoWriter::flush()
{
	int res = avcodec_send_frame(pCodecCtx, NULL);
	if (res < 0) {
		av_log(nullptr, AV_LOG_FATAL, "Error sending a frame for encoding.\n");
		return -1;
	}
	while (res >= 0) {
		//Read encoded data from the encoder
		res = avcodec_receive_packet(pCodecCtx, pkt);
		if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) {
			break;
		}
		if (res < 0) {
			av_log(nullptr, AV_LOG_FATAL, "Error during encoding.\n");
			return -1;
		}
		if (res >= 0) {
			pkt->stream_index = 0;
			//Write a packet to an output media file
			av_write_frame(pFmtCtx, pkt);
			av_packet_unref(pkt);
		}
	}
	//Write the stream trailer to an output media file and free the file private data
	av_write_trailer(pFmtCtx);
	av_frame_free(&pFrame);
	
	return 0;
}

