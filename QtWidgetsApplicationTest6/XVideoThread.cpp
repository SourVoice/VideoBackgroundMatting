#include "ui_mainwindow.h"
#include "XVideoThread.h"
#include "XFFmpeg.h"
#include "video.h"
#include "mainwindow.h"
#include <iostream>
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/imgproc/imgproc_c.h"//for cvSmooth

bool isExit = false;
XVideoThread::XVideoThread()
{
    //注册所有文件格式
    avdevice_register_all();
}

XVideoThread::~XVideoThread()
{
}

void XVideoThread::run()
{
    Mat cv_frame;

	while (!isExit) {



        //ffmpeg 解码转码视频
        int i = 0;
        int ret = 0;
        AVFormatContext* fmt_ctx = NULL;
        AVCodecContext* dec_ctx = NULL;
        AVCodec* dec = NULL;

        //分配一个avformat
        fmt_ctx = avformat_alloc_context();
        if (fmt_ctx == NULL)
        {
            QMessageBox::warning(nullptr, "提示", "fmt_ctx分配错误",
                QMessageBox::Yes | QMessageBox::Yes);
            exit(1);
        }

        //打开文件，解封装
        if (avformat_open_input(&fmt_ctx, in_file, NULL, NULL) != 0)
        {
            //printf("%s", in_file);
            QMessageBox::warning(nullptr, "提示", "打开文件解封装错误",
                QMessageBox::Yes | QMessageBox::Yes);

            exit(1);
        }

        //查找文件的相关流信息
        if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
        {
            //printf("find stream fail");
            QMessageBox::warning(nullptr, "提示", "查找文件的相关流信息错误",
                QMessageBox::Yes | QMessageBox::Yes);

            exit(1);
        }

        //输出格式信息
        av_dump_format(fmt_ctx, 0, in_file, 0);

        //查找解码信息
        int stream_index = -1;
        for (i = 0; i < fmt_ctx->nb_streams; i++) {
            if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                stream_index = i;
                break;
            }
        }

        if (stream_index == -1)
            printf("find stream fail");

        //保存解码器
        dec = (AVCodec*)avcodec_find_decoder(fmt_ctx->streams[stream_index]->codecpar->codec_id);
        if (dec == NULL)
            printf("find codec fail");

        /* create decoding context 保存解码后数据*/
        dec_ctx = avcodec_alloc_context3(dec);
        if (!dec_ctx)
            printf("avcodec_alloc_context3 failed\n");
        avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[stream_index]->codecpar);

        if (avcodec_open2(dec_ctx, dec, NULL) < 0)
            printf("can't open codec");


        //创建packet,用于存储解码前的数据
        AVPacket* packet = av_packet_alloc();;
        av_init_packet(packet);


        //创建Frame，用于存储解码后的数据
        AVFrame* frame = av_frame_alloc();

        int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, dec_ctx->width, dec_ctx->height, 1);

        //注意要用av_malloc
        uint8_t* buffer = (uint8_t*)av_malloc(sizeof(uint8_t) * buffer_size);

        av_image_fill_arrays(frame->data, frame->linesize, buffer, AV_PIX_FMT_RGB24, dec_ctx->width, dec_ctx->height, 1);

        //打开转码器
        struct SwsContext* convert_ctx = sws_alloc_context();

        //设置转码参数
        convert_ctx = sws_getContext(dec_ctx->width, dec_ctx->height,
                                     dec_ctx->pix_fmt,
                                     dec_ctx->width, dec_ctx->height, AV_PIX_FMT_RGB32,
                                     SWS_BICUBIC, NULL, NULL, NULL);

        //初始化转码器

        //sws_init_context(convert_ctx);
        //while循环，每次读取一帧，并转码

        while (av_read_frame(fmt_ctx, packet) >= 0) {

            if (packet->stream_index == stream_index) {

                //QMessageBox::warning(nullptr, "提示", QString::number(stream_index),
                //                     QMessageBox::Yes | QMessageBox::Yes);
                ret = avcodec_send_packet(dec_ctx, packet);
                //avcodec_send_frame();

                if (ret < 0) {
                    fprintf(stderr, "Error sending a packet for decoding\n");

                    break;
                }

                while (ret >= 0) {
                    ret = avcodec_receive_frame(dec_ctx, frame);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                        break;
                    else if (ret < 0) {
                        fprintf(stderr, "Error during decoding\n");
                        break;
                    }
                    fflush(stdout);
                }
                //QMessageBox::warning(nullptr, "提示", "显示一帧",
                //                     QMessageBox::Yes | QMessageBox::Yes);

            }
            //release resource
            av_packet_unref(packet);
        }

        //释放转码结构体
        sws_freeContext(convert_ctx);
        //
        av_frame_free(&frame);

        //关闭解码器
        avcodec_close(dec_ctx);


	}
}