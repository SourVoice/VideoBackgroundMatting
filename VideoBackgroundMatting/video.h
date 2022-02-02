#ifndef VIDEO_H
#define VIDEO_H
#pragma execution_character_set("utf-8")

#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//using namespace cv;
#include <QObject>

class video:public QObject
{
    Q_OBJECT
public:
    explicit video(QObject *parent = 0);
    ~video();
signals:
    //void complete();
public slots:
    //QImage mat2image(const cv::Mat& mat);
};

#endif // VIDEO_H
