#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")

#include "ui_mainwindow.h"
#include "XFFmpeg.h"
#include "BGProcess.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
//Qt
#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QToolButton>
#include <QApplication>
#include <QSpinBox>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QLabel>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QPaintDevice>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QThread>
#include <QObject>
#include <QtCore/qmath.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:

	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

	//图片部分
	int index = 0;                                                  //图片index
	QString origin_path;                                            //目前处理的图片的原图路径
	QStringList srcDirPathList;                                     //图片list

	QImage setRGB(QImage image, int value_r, int value_g, int value_b);//调整rgb
	QImage AdjustContrast(QImage image, int value);                 //调整对比度
	QImage ImageCenter(QImage  qimage, QLabel* qLabel);              //调整图片比例
	QImage AdjustSaturation(QImage image, int value);               //调整饱和度
	QImage gray(QImage image);                                      //灰度化
	QImage AverageFiltering(QImage image);                          //均值滤波
	QImage EdgeFliter(QImage image);                                //边缘检测
	QImage OriginalPlusEdgeFliter(QImage images);                   //原图+边缘检测复合调用函数
	QImage gamma(QImage image);                                     //伽马变换

	//视频部分
	cv::Mat QImage2cvMat(QImage image);
	QImage Mat2QImage(const cv::Mat& mat);
	cv::Mat Avframe2cvMat(AVFrame* avframe, int w, int h);
	cv::Mat Mosaic(cv::Mat image);                                  //马赛克
	QString stom(int s);                                            //video播放,时间转换函数
	XFFmpeg* ffmpeg = nullptr;                                      //ffmpeg解码
	//cv::VideoCapture capture;                                     //用来读取视频结构(仅作测试使用)
	QString video_path;                                             //视频路径

	int type = 0;                                                   //视频操作类型
	double currentVideoTime = 0;
	double totalVideoTime = 0;
	bool isPlay = false;

	//视频处理
	BGProcess* process = nullptr;


	//其他
public:
	Ui::MainWindow* ui;
	QMessageBox customMsgBox;

	bool language = true;

signals:
	void receiveIsPlay(const bool& isPlay);

private slots:

	void on_action_Dock_triggered();

	void on_action_Open_triggered();

	void on_pushButton_clicked();

	void onDisplayImage(const QImage& image);
	void onGetCurrentVideoTime(const double& time);
	void onGetTotalVideoTime(const double& time);

	//void updatePosition();

	void on_action_Save_triggered();

	void on_pushButton_gray_clicked();

	void on_pushButton_junzhi_clicked();

	void on_action_L_triggered();

	void on_horizontalSlider_valueChanged(int value);

	void on_pushButton_junzhi_2_clicked();

	void on_pushButton_junzhi_3_clicked();

	void on_horizontalSlider_2_valueChanged(int value);

	void on_pushButton_save_clicked();

	void on_horizontalSlider_R_valueChanged(int value);

	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_pushButton_origin_clicked();

	void on_horizontalSlider_G_valueChanged(int value);

	void on_horizontalSlider_B_valueChanged(int value);

	void on_pushButton_gamma_clicked();

	void on_horizontalSlider_erzhi_valueChanged(int value);

	void on_horizontalSlider_duibi_valueChanged(int value);

	void on_horizontalSlider_baohe_valueChanged(int value);

	void on_action_V_triggered();

	void on_pushButton_5_clicked();//剪切视频中对象

	void on_pushButton_6_clicked();

	void on_pushButton_7_clicked();

	void on_pushButton_8_clicked();

	//void on_VideohorizontalSlider_2_valueChanged(int value);

	void on_pushButton_9_clicked();

	void on_pushButton_10_clicked();

	void on_pushButton_11_clicked();

	void on_horizontalSlider_suofang_valueChanged(int value);

	void on_pushButton_2_clicked();

	void on_action_H_triggered();

	void on_action_J_triggered();

	void on_action_B_triggered();

	void on_action_G_triggered();

	void on_action_Y_triggered();

	void on_action_About_triggered();

	void on_pushButton_turnleft_clicked();

	void on_pushButton_turnright_clicked();

	void on_pushButton_turnleft_2_clicked();

	void on_pushButton_turnleft_3_clicked();


};
#endif // MAINWINDOW_H
