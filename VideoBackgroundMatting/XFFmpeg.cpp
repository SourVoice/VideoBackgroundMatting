#include "XFFmpeg.h"
using namespace cv;

#define TIMEMS      qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))
#define STRDATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

XFFmpeg::XFFmpeg(QObject* parent) :QThread(parent)
{
	errorbuff[0] = '\0';
	in_file = nullptr;
	//av_register_all();//注册FFMpeg的库(高版本ffmpeg库不用调用)

	buffer = nullptr;
	avPacket = nullptr;       
	stream = nullptr;			
	avFrame = nullptr;            
	avFormatContext = nullptr;
	videoCodecCtx = nullptr;     
	audioCodec = nullptr;     
	swsContext = nullptr;

	Options = nullptr;
	videoCodec = nullptr;
	audioDecoder = nullptr;
	//connect(this, &QThread::finished, this, &QObject::deleteLater);

	XFFmpeg::initalizeLib();

}

//软件中 只需初始化一次
void XFFmpeg::initalizeLib()
{
	static QMutex mutex;
	QMutexLocker locker(&mutex);
	static bool isInit = true;
	if (!isInit) {
		isInit = true;
		qDebug() << TIMEMS << "init ffmpeg lib ok" << " version:" << FFMPEG_VERSION;
	}
}

void XFFmpeg::Open(const QString& fileInput) {
	this->path = fileInput;
	std::string str = std::string(fileInput.toLocal8Bit());
	this->in_file = (char*)calloc(fileInput.size(), sizeof(char));
	strcpy(in_file, (char*)str.c_str());
	//QString 转为 char*
	this->init();
}

bool XFFmpeg::init()
{
	Close();		//打开前先关闭清理
	mutex.lock();	//锁

	//在打开码流前指定各种参数比如:探测时间/超时时间/最大延时等
	//设置缓存大小,1080p可将值调大
	av_dict_set(&Options, "buffer_size", "8192000", 0);
	//以tcp方式打开,如果以udp方式打开将tcp替换为udp
	av_dict_set(&Options, "rtsp_transport", "tcp", 0);
	//设置超时断开连接时间,单位微秒,3000000表示3秒
	av_dict_set(&Options, "stimeout", "3000000", 0);
	//设置最大时延,单位微秒,1000000表示1秒
	av_dict_set(&Options, "max_delay", "1000000", 0);
	//自动开启线程数
	av_dict_set(&Options, "threads", "auto", 0);


	//打开视频流
	avFormatContext = avformat_alloc_context();

	//检测avFormatContext初始化分配是否错误
	if (avFormatContext == NULL)
	{
		qDebug() << TIMEMS << "avFormatContext分配失败";
		exit(1);
	}

	//打开文件，解封装
	if (avformat_open_input(&avFormatContext, in_file, NULL, NULL) != 0)
	{
		int result = avformat_open_input(&avFormatContext, in_file, NULL, NULL);
		av_strerror(result, errorbuff, sizeof(errorbuff));//错误信息
		printf("open %s failed :%s\n", in_file, errorbuff);
		//printf("%s", fileInput);
		qDebug() << TIMEMS << "open file error 打开文件解封装错误" << in_file;
		exit(1);
	}

	//释放设置参数
	if (Options != nullptr) {
		av_dict_free(&Options);
	}

	//获取文件的流信息
	if (avformat_find_stream_info(avFormatContext, NULL) < 0)
	{
		qDebug() << TIMEMS << "find stream info error!\n查找文件的相关流信息错误";
		exit(1);
	}

	totalVideoTime = avFormatContext->duration / (AV_TIME_BASE);//获取视频的总时间

	//视频流
	if (1) {
		//查找解码信息
		for (int i = 0; i < avFormatContext->nb_streams; i++) {
			if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
				videoStreamIndex = i;
				break;
			}
		}

		if (videoStreamIndex == -1)
			qDebug() << TIMEMS << "find stream fail";

		//获取流解码器
		videoCodec = (AVCodec*)avcodec_find_decoder(avFormatContext->streams[videoStreamIndex]->codecpar->codec_id);
		if (videoCodec == NULL)
			qDebug() << TIMEMS << "find videoCodec error!";

		//保存解码器上下文
		videoCodecCtx = avcodec_alloc_context3(videoCodec);
		if (!videoCodecCtx)
			qDebug() << TIMEMS << "avcodec_alloc_context3 error!";

		//设置加速解码
		videoCodecCtx->lowres = videoCodec->max_lowres;
		videoCodecCtx->flags2 |= AV_CODEC_FLAG2_FAST;
		avcodec_parameters_to_context(videoCodecCtx, avFormatContext->streams[videoStreamIndex]->codecpar);

		//打开视频解码器
		if (avcodec_open2(videoCodecCtx, videoCodec, NULL) < 0)
			qDebug() << TIMEMS << "can't open codec!";

		//获取分辨率大小
		videoWidth = videoCodecCtx->width;
		videoHeight = videoCodecCtx->height;

		//视频流信息
		QString videoInfo = QString("视频流信息 -> 索引: %1  解码: %2  格式: %3  时长: %4 秒  分辨率: %5*%6")
			.arg(videoStreamIndex).arg(videoCodec->name).arg(avFormatContext->iformat->name)
			.arg((avFormatContext->duration) / 1000000).arg(videoWidth).arg(videoHeight);
		qDebug() << TIMEMS << videoInfo;
	}

	AVStream *s = avFormatContext->streams[videoStreamIndex];
	frame_rate = s->avg_frame_rate.num / s->avg_frame_rate.den;
	
	//定义像素格式
	AVPixelFormat srcFormat = AVPixelFormat::AV_PIX_FMT_YUV420P;
	AVPixelFormat dstFormat = AVPixelFormat::AV_PIX_FMT_RGB32;
	//通过解码器获取解码格式
	srcFormat = videoCodecCtx->pix_fmt;
	//avPacket, 存储解码前的数据
	avPacket = av_packet_alloc();
	av_init_packet(avPacket);

	//avFrame，存储解码后的数据(一帧数据)
	avFrame = av_frame_alloc();
	
	//为avFrame开辟缓存
	int buffer_size = av_image_get_buffer_size(dstFormat, videoWidth, videoHeight, 1);
	buffer = (uint8_t*)av_malloc(sizeof(uint8_t) * buffer_size);
	av_image_fill_arrays(avFrame->data, avFrame->linesize, buffer, dstFormat, videoWidth, videoHeight, 1);

	//打开转码器
	swsContext = sws_alloc_context();

	//设置转码参数,转换图像
	swsContext = sws_getContext(videoWidth, videoHeight,
								srcFormat,				/*srcFormat*/
								videoWidth, videoHeight,
								dstFormat,				/*distFormat*/
								SWS_BICUBIC,
								NULL, NULL, NULL);
	
	//输出格式信息
	av_dump_format(avFormatContext, 0, in_file, 0);
	mutex.unlock();
	return true;

}

void XFFmpeg::run()
{
	
	QImage image(videoWidth, videoHeight, QImage::Format_RGB32);
	int outputLineSize[3];                                                                         //构造AVFrame到QImage所需要的数据
	av_image_fill_linesizes(outputLineSize, AV_PIX_FMT_RGB32, videoWidth);
	uint8_t* outputDst[] = { image.bits() };
	while (1)
	{	
		//QCoreApplication::processEvents();   //处理事件
		
		// 检测是否停止
		{
			QMutexLocker locker(&mutex);
			if (Abort)
				break;
		}
		
		//根据标志位执行初始化操作
		if (!isPlay) {
			continue;
		}
		QMutexLocker locker(&mutex);// 此处加锁,防止访问冲突

		time.restart();
		AVFrame* copyFrame = av_frame_alloc();
		copyFrame->format = avFrame->format;
		copyFrame->width = avFrame->width;
		copyFrame->height = avFrame->height;
		copyFrame->channels = avFrame->channels;
		copyFrame->channel_layout = avFrame->channel_layout;
		copyFrame->nb_samples = avFrame->nb_samples;
		av_frame_get_buffer(copyFrame, 32);
		av_frame_copy(copyFrame, avFrame);
		av_frame_copy_props(copyFrame, avFrame);
		if (!copyFrame) {
			qDebug() << TIMEMS << "null copyImage";
			break;
		}
		//处理一帧
		if (av_read_frame(avFormatContext, avPacket) >= 0) {
			if (avPacket->stream_index == videoStreamIndex) {
				int ret = avcodec_send_packet(videoCodecCtx, avPacket);

				if (ret < 0) {
					fprintf(stderr, "Error sending a packet for decoding\n");
					qDebug() << stderr << "Error sending a packet for decoding\n";
					break;
				}

				while (ret >= 0) {
					ret = avcodec_receive_frame(videoCodecCtx, avFrame);
					if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
						break;
					else if (ret < 0) {
						fprintf(stderr, "Error during decoding\n");
						qDebug() << stderr << "Error during decoding\n\n";
						break;
					}
					fflush(stdout);

					if (totalVideoTime)
						emit receiveTotalVideoTime(totalVideoTime);
					currentVideoTime = avFrame->pts * AVRationalr2Double(avFormatContext->streams[avPacket->stream_index]->time_base);
					if(currentVideoTime)
						emit receiveCurrentVideoTime(currentVideoTime);


					sws_scale(swsContext, (const uint8_t* const*)avFrame->data, avFrame->linesize, 0, videoHeight, outputDst, outputLineSize);
					
					//QImage image(copyFrame->data[0], videoWidth, videoHeight, QImage::Format_RGB888);
					//QImage image((uchar*)buffer, videoWidth, videoHeight, QImage::Format_RGB32);
					if (!image.isNull()) {
						emit receiveImage(image);
					}
					else {
						qDebug() << TIMEMS << "Corrupted Qimage";
					}
					msleep(frame_rate);
				}
			}
		}
		av_frame_free(&copyFrame);
	}
	releaseMem();
	free(outputDst);
}

//Avframe*转为openCV中的Mat
cv::Mat XFFmpeg::Avframe2cvMat(AVFrame* avframe, int w, int h)
{

	if (w <= 0) w = avframe->width;
	if (h <= 0) h = avframe->height;
	struct SwsContext* sws_ctx = NULL;
	sws_ctx = sws_getContext(avframe->width, avframe->height, (enum AVPixelFormat)avframe->format,
		w, h, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

	cv::Mat mat;
	mat.create(cv::Size(w, h), CV_8UC3);
	AVFrame* bgr24frame = av_frame_alloc();
	bgr24frame->data[0] = (uint8_t*)mat.data;

	int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, w, h, 1);

	//注意要用av_malloc
	uint8_t* buffer = (uint8_t*)av_malloc(sizeof(uint8_t) * buffer_size);

	av_image_fill_arrays(bgr24frame->data, bgr24frame->linesize, buffer, AV_PIX_FMT_RGB24, w, h, 1);


	//avpicture_fill((AVPicture*)bgr24frame, bgr24frame->data[0], AV_PIX_FMT_BGR24, w, h);
	sws_scale(sws_ctx,
		(const uint8_t* const*)avframe->data, avframe->linesize,
		0, avframe->height, // from cols=0,all rows trans
		bgr24frame->data, bgr24frame->linesize);

	av_free(bgr24frame);
	sws_freeContext(sws_ctx);
	return mat;
}

void XFFmpeg::releaseMem()
{
	if (buffer != nullptr) {
		av_free(buffer);
		buffer = nullptr;
	}

	if (in_file != nullptr)
	{
		free(in_file);
		in_file = nullptr;
	}
	//释放转码结构体
	if (swsContext != nullptr) {
		sws_freeContext(swsContext);
		swsContext = nullptr;
	}

	//释放解码前该帧
	if (avPacket != nullptr) {
		av_packet_unref(avPacket);
		avPacket = nullptr;
	}

	//释放解码后该帧
	if (avFrame != nullptr) {
		av_frame_free(&avFrame);
		avFrame = nullptr;
	}

	//释放上下文结构体
	if (videoCodecCtx != nullptr) {
		avcodec_close(videoCodecCtx);
		videoCodecCtx = nullptr;
	}

	if (audioCodec != nullptr) {
		avcodec_close(audioCodec);
		audioCodec = nullptr;
	}

	if (avFormatContext != nullptr) {
		avformat_close_input(&avFormatContext);
		avFormatContext = nullptr;
	}

	if (Options != nullptr) {
		av_dict_free(&Options);
		Options = nullptr;
	}
	qDebug() << TIMEMS << "close ffmpeg ok";
}

XFFmpeg::~XFFmpeg()
{
	Stop();
	quit();
	wait();
	//线程结束释放资源
	qDebug() << TIMEMS << "isFinished";
}

void XFFmpeg::Close()
{
	mutex.lock();								//需要上锁，以防多线程中你这里在close，另一个线程中在读取，
	if (avFormatContext) avformat_close_input(&avFormatContext);//关闭ic上下文
	mutex.unlock();
}

void XFFmpeg::Stop()
{
	mutex.lock();
	this->Abort = true;
}

void XFFmpeg::Play()
{

	mutex.unlock();
	this->Abort = false;
}

std::string XFFmpeg::GetError()
{

	mutex.lock();
	std::string re = this->errorbuff;
	mutex.unlock();
	return re;
}

double XFFmpeg::AVRationalr2Double(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

double XFFmpeg::getFrameRate(void)
{

	/*mutex.lock();
	stream = avFormatContext->streams[avPacket->stream_index];

	double frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
	mutex.unlock();*/

	return frame_rate;
}

void XFFmpeg::onGetIsPlay(const bool& isPlay)
{
	this->isPlay = isPlay;
}
