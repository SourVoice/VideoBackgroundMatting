#include "pch.h"
#include "VideoReader.h"


VideoReader::VideoReader(std::string filename)
{
	in_filename = filename;
	init();
}


VideoReader::~VideoReader()
{

	av_frame_free(&frame);
	av_packet_free(&pkt);
	avcodec_free_context(&codec_ctx);
	avformat_free_context(ifmt_ctx);
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

	// 1���򿪶�ý���ļ�
	ret = avformat_open_input(&ifmt_ctx, in_filename.data(), NULL, NULL);
	if (ret < 0)
	{
		std::cout << "avformat_open_input error,ret=" << ret << std::endl;
		return -1;
	}

	ret = avformat_find_stream_info(ifmt_ctx, NULL);
	if (ret < 0)
	{
		std::cout << "avformat_find_stream_info error,ret=" << ret << std::endl;
		return -5;
	}
	av_dump_format(ifmt_ctx, 0, in_filename.data(), 0);

	int video_stream_idx = -1;
	for (size_t i = 0; i < ifmt_ctx->nb_streams; i++)
	{
		if (ifmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			//avcodec_parameters_to_context(codec_ctx, ifmt_ctx->streams[i]->codecpar);
			video_stream_idx = i;
			break;
		}
	}
	if (video_stream_idx == -1) {
		std::cout << "no video stream found" << std::endl;
		return -1;
	}
	// 2��find������

	codec = (AVCodec*)avcodec_find_decoder(ifmt_ctx->streams[video_stream_idx]->codecpar->codec_id);
	if (codec == NULL) {
		std::cout << "no decoder available" << std::endl;
		return -1;
	}

	// 3�����������������
	codec_ctx = avcodec_alloc_context3(codec);
	if (codec_ctx == NULL)
	{
		std::cout << "avcodec_alloc_context3 error" << std::endl;
	}

	// 4����ȡ���������֧��mp4/H264�ļ�,�⼸�д���ǳ���Ҫ�������ܽ���mp4��yuv
	//���ü��ٽ���
	codec_ctx->lowres = codec->max_lowres;
	codec_ctx->flags2 |= AV_CODEC_FLAG2_FAST;
	avcodec_parameters_to_context(codec_ctx, ifmt_ctx->streams[video_stream_idx]->codecpar);

	// 5��open������
	ret = avcodec_open2(codec_ctx, codec, NULL);
	if (ret < 0)
	{
		std::cout << "avcodec_open2 error, ret=" << ret << std::endl;
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
		// �������Ƶ
		if (ifmt_ctx->streams[pkt->stream_index]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			if (pkt->size)
			{
				ret = avcodec_send_packet(codec_ctx, pkt);
				if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					std::cout << "avcodec_send_packet: " << ret << std::endl;
				}
				ret = avcodec_receive_frame(codec_ctx, frame);
				if (ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					std::cout << "avcodec_receive_packet: " << ret << std::endl;
				}
				img = AVFrame2Img(frame);
				av_packet_unref(pkt);
				return 1;
			}
			av_packet_unref(pkt);
		}
	}
	return -1;
}

cv::Mat VideoReader::AVFrame2Img(AVFrame * pFrame)
{
	int frameHeight = pFrame->height;
	int frameWidth = pFrame->width;

	//��������yuv���ݵ�buffer
	uchar* pDecodedBuffer = (uchar*)malloc(frameHeight*frameWidth * sizeof(uchar) * 3 / 2);

	//��AVFrame�л�ȡyuv420p���ݣ������浽buffer
	int i, j, k;
	//����y����
	for (i = 0; i < frameHeight; i++)
	{
		memcpy(pDecodedBuffer + frameWidth * i,
			pFrame->data[0] + pFrame->linesize[0] * i,
			frameWidth);
	}
	//����u����
	for (j = 0; j < frameHeight / 2; j++)
	{
		memcpy(pDecodedBuffer + frameWidth * i + frameWidth / 2 * j,
			pFrame->data[1] + pFrame->linesize[1] * j,
			frameWidth / 2);
	}
	//����v����
	for (k = 0; k < frameHeight / 2; k++)
	{
		memcpy(pDecodedBuffer + frameWidth * i + frameWidth / 2 * j + frameWidth / 2 * k,
			pFrame->data[2] + pFrame->linesize[2] * k,
			frameWidth / 2);
	}
	//���ͼ������ڴ�
	cv::Mat img = cv::Mat(frameHeight * 3 / 2, frameWidth, CV_8UC1, pDecodedBuffer);
	cv::Mat bgr;
	cv::cvtColor(img, bgr, cv::COLOR_YUV420p2RGB);
	//�ͷ�buffer
	free(pDecodedBuffer);
	return bgr;
}


int main()
{
	VideoReader reader = VideoReader("test_out_opendefenc.mp4");
	cv::Mat frame;
	while (reader.read(frame) >= 0)
	{
		cv::imshow("", frame);
		cv::waitKey(1);
	}
}