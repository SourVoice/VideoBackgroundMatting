#include "pch.h"
#include "VideoWriter.h"


VideoWriter::VideoWriter(int w, int h, std::string file_out)
{
	input_w = w;
	input_h = h;
	filename_out = file_out;
	frame_cnt = 0;
	int32_t y_size = w * h * 3 / 2;
	int32_t uv_size = y_size / 2;
	y_buf = (uint8_t*)_aligned_malloc(y_size, 64);
	uv_buf = (uint8_t*)_aligned_malloc(uv_size, 64);
	init();
}


VideoWriter::~VideoWriter()
{
	_aligned_free(y_buf);
	_aligned_free(uv_buf);

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
	vStream->codecpar->bit_rate = BIT_RATE;
	vStream->codecpar->format = AV_PIX_FMT_NV12;
	vStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
	vStream->codecpar->codec_id = AV_CODEC_ID_H264;
	vStream->codecpar->sample_aspect_ratio = av_make_q(4, 3);  // 这里设置4：3，视频就可以按照16：9显示

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
	pCodecCtx->time_base.den = FRAME_RATE;
	pCodecCtx->bit_rate = BIT_RATE;

	pCodecCtx->qmin = 10;
	pCodecCtx->qmax = 51;
	pCodecCtx->qcompress = 0.6;
	av_opt_set(pCodecCtx->priv_data, "preset", "fast", 0);

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

	AVDictionary* opt = NULL;

	int frame_rate = FRAME_RATE;
	av_dict_set_int(&opt, "video_track_timescale", frame_rate, 0);

	//Allocate the stream private data and write the stream header to an output media file
	avformat_write_header(pFmtCtx, &opt);

	return 1;
}

int VideoWriter::write(cv::Mat bgr)
{
	bgr2nv12(bgr);
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
}



void VideoWriter::bgr2nv12(cv::Mat bgr)
{
	cv::Mat yuv_mat;
	cv::cvtColor(bgr, yuv_mat, cv::COLOR_BGR2YUV_I420);

	uint8_t *yuv = yuv_mat.ptr<uint8_t>();

	int32_t y_height = yuv_mat.rows * 2 / 3;
	int32_t y_width = yuv_mat.cols;

	// copy y data
	int32_t y_size = y_height * y_width;
	memcpy(y_buf, yuv, y_size);

	// copy uv data
	int32_t u_size = y_size / 4;

	uint8_t *uv_p = uv_buf;
	uint8_t *u_data = yuv + y_size;
	uint8_t *v_data = u_data + u_size;


	for (int32_t j = 0; j < u_size; j++) {
		*uv_p++ = *u_data++;
		*uv_p++ = *v_data++;
	}
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

