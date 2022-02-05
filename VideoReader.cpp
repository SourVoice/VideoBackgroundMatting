#include "pch.h"
#include "VideoReader.h"


VideoReader::VideoReader(string filename)
{
	this->filename = filename;
	errorbuff[0] = '\0';
	buffer = nullptr;
	avPacket = nullptr;
	stream = nullptr;
	avFrame = nullptr;
	avFormatContext = nullptr;
	videoCodecCtx = nullptr;
	swsContext = nullptr;

	Options = nullptr;
	videoCodec = nullptr;
	init();
}


VideoReader::~VideoReader()
{
}

void VideoReader::init()
{
	//�ڴ�����ǰָ�����ֲ�������:̽��ʱ��/��ʱʱ��/�����ʱ��
	//���û����С,1080p�ɽ�ֵ����
	av_dict_set(&Options, "buffer_size", "8192000", 0);

	//����Ƶ��
	avFormatContext = avformat_alloc_context();

	//���avFormatContext��ʼ�������Ƿ����
	if (avFormatContext == NULL)
	{
		cout  << "avFormatContext����ʧ��";
		exit(1);
	}

	//���ļ������װ
	int result = avformat_open_input(&avFormatContext, filename.c_str(), NULL, NULL);
	if (result != 0)
	{
		av_strerror(result, errorbuff, sizeof(errorbuff));//������Ϣ
		printf("open %s failed :%s\n", filename.c_str(), errorbuff);
		exit(1);
	}

	//�ͷ����ò���
	if (Options != NULL) {
		av_dict_free(&Options);
	}

	//��ȡ�ļ�������Ϣ
	if (avformat_find_stream_info(avFormatContext, NULL) < 0)
	{
		cout  << "find stream info error!\n�����ļ����������Ϣ����";
		exit(1);
	}

	totalVideoTime = avFormatContext->duration / (AV_TIME_BASE);//��ȡ��Ƶ����ʱ��

	//��Ƶ��

	//���ҽ�����Ϣ
	for (int i = 0; i < avFormatContext->nb_streams; i++) {
		if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStreamIndex = i;
			break;
		}
	}

	if (videoStreamIndex == -1)
	{
		cout << "find stream fail";
		exit(1);
	}
	//��ȡ��������
	videoCodec = (AVCodec*)avcodec_find_decoder(avFormatContext->streams[videoStreamIndex]->codecpar->codec_id);
	if (videoCodec == NULL)
	{
		cout << "find videoCodec error!";
		exit(1);
	}

	//���������������
	videoCodecCtx = avcodec_alloc_context3(videoCodec);
	if (!videoCodecCtx)
	{
		cout << "avcodec_alloc_context3 error!";
		exit(1);
	}

	//���ü��ٽ���
	videoCodecCtx->lowres = videoCodec->max_lowres;
	videoCodecCtx->flags2 |= AV_CODEC_FLAG2_FAST;
	avcodec_parameters_to_context(videoCodecCtx, avFormatContext->streams[videoStreamIndex]->codecpar);

	//����Ƶ������
	if (avcodec_open2(videoCodecCtx, videoCodec, NULL) < 0)
	{
		cout << "can't open codec!";
		exit(1);
	}

	//��ȡ�ֱ��ʴ�С
	videoWidth = videoCodecCtx->width;
	videoHeight = videoCodecCtx->height;

	
	//�������ظ�ʽ
	AVPixelFormat srcFormat = AVPixelFormat::AV_PIX_FMT_YUV420P;
	AVPixelFormat dstFormat = AVPixelFormat::AV_PIX_FMT_RGB32;
	//ͨ����������ȡ�����ʽ
	srcFormat = videoCodecCtx->pix_fmt;

	//avPacket, �洢����ǰ������
	avPacket = av_packet_alloc();

	//avFrame���洢����������(һ֡����)
	avFrame = av_frame_alloc();

	//ΪavFrame���ٻ���
	int buffer_size = av_image_get_buffer_size(dstFormat, videoWidth, videoHeight, 1);
	buffer = (uint8_t*)av_malloc(sizeof(uint8_t) * buffer_size);
	av_image_fill_arrays(avFrame->data, avFrame->linesize, buffer, dstFormat, videoWidth, videoHeight, 1);

	//��ת����
	swsContext = sws_alloc_context();

	//����ת�����,ת��ͼ��
	swsContext = sws_getContext(videoWidth, videoHeight,
		srcFormat,				/*srcFormat*/
		videoWidth, videoHeight,
		dstFormat,				/*distFormat*/
		SWS_BICUBIC,
		NULL, NULL, NULL);

	//�����ʽ��Ϣ
	av_dump_format(avFormatContext, 0, filename.c_str(), 0);

}

void VideoReader::read()
{
	cv::Mat mat = cv::Mat(videoHeight, videoWidth, CV_8UC3);
	while(read_a_frame(mat) >= 0)
	{
		//cv::imshow("", mat);
		cv::waitKey(0);
	}
	//av_frame_free(&avFrameCopy);
}

int VideoReader::read_a_frame(cv::Mat &mat)
{
	if (av_read_frame(avFormatContext, avPacket) < 0)
	{
		return -1;
	}
	if (avPacket->stream_index == videoStreamIndex) {
		if (avcodec_send_packet(videoCodecCtx, avPacket) != 0)
		{
			cout << "failed to send video stream packet" << endl;
			return -1;
		}
		if (avcodec_receive_frame(videoCodecCtx, avFrame) != 0) {
			cout << "failed to receive video frame" << endl;
			return 1;
		}
		cout << "decoding " << (++frame_idx) << " frame" << endl;
		sws_scale(swsContext, avFrame->data, avFrame->linesize, 0, videoHeight, avFrame->data, avFrame->linesize);
		
		avframe2cvmat(avFrame);

		return 0;

	}
	
}

void VideoReader::avframe2cvmat(AVFrame * avframe)
{
	int width = avframe->width, height = avframe->height;
	int channel_size = width * height;
	
	cv::Mat r_channel = cv::Mat(height, width, CV_8UC1);
	cv::Mat g_channel = cv::Mat(height, width, CV_8UC1);
	cv::Mat b_channel = cv::Mat(height, width, CV_8UC1);
	vector<cv::Mat> merge{ r_channel, g_channel, b_channel };
	cv::Mat rgb;

	cout << r_channel.elemSize();
	cout << strlen((const char *)avframe->data[0]);
	memcpy(r_channel.data, avframe->data[0], channel_size);
	memcpy(g_channel.data, avframe->data[1], channel_size);
	memcpy(b_channel.data, avframe->data[2], channel_size);

	cv::merge(merge, rgb);
	//cv::Mat tmp_img = cv::Mat::zeros(height * 3 / 2, width, CV_8UC1);
	//memcpy(tmp_img.data, avframe->data[0], width*height);
	//memcpy(tmp_img.data + width * height, avframe->data[1], width*height / 4);
	//memcpy(tmp_img.data + width * height * 5 / 4, avframe->data[2], width*height / 4);

	///*cv::imshow("yuv_show", tmp_img);*/
	//cv::Mat bgr;
	//cv::cvtColor(tmp_img, bgr, cv::COLOR_YUV2BGR_I420);
	cv::imshow("bgr_show", rgb);
	//cv::waitKey(0);
}


