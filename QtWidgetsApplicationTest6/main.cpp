#include <windows.h>
#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(NULL);
    a.installTranslator(&translator);
    QPixmap pixmap(":/myImage/images/loading.gif");
        QSplashScreen splash(pixmap);
        QLabel label(&splash);
        QMovie mv(":/myImage/images/loading.gif");
        label.setMovie(&mv);
        mv.start();
        splash.show();
        splash.setCursor(Qt::BlankCursor);
        for(int i=0; i<50; i+=mv.speed())
        {
            QCoreApplication::processEvents();
            Sleep(10*static_cast<uint64>(mv.speed()));
        }

        //set qss style
        QString qss;
        QFile qssFile(":/myQss.qss");
        qssFile.open(QFile::ReadOnly);
        if (qssFile.isOpen())
        {
            qss = QLatin1String(qssFile.readAll());
            qApp->setStyleSheet(qss);
            qssFile.close();
        }

        MainWindow w;
        w.setWindowTitle(QObject::tr("ͼ����Ƶ��������"));
        w.show();
        splash.finish(&w);
    return a.exec();
}