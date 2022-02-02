#pragma once
#include "mainwindow.h"
#include <qthread.h>
class XVideoThread :public QThread, public MainWindow
{
    public:
        static XVideoThread* Get() {
            static XVideoThread vt;
            return &vt;
        }
        void run();
        XVideoThread();
        virtual ~XVideoThread();
};

