#include "mainwindow.h"
#include <windows.h>
#include <QApplication>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QTranslator>
#include <QFile>

int main(int argc, char* argv[])
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
	for (int i = 0; i < 100; i += mv.speed())
	{
		QCoreApplication::processEvents();
		Sleep(10 * static_cast<uint64>(mv.speed()));
	}

	a.setStyle(QStyleFactory::create("fusion"));
	
	// ���ó�������ʱ��������ͼ��
	QIcon icon(":/menubar/icon/menuBar/videoBlock.png");
	a.setWindowIcon(icon);

	MainWindow w;
	w.setWindowTitle(QObject::tr("ͼ����Ƶ������"));
	w.show();
	splash.finish(&w);
	return a.exec();
}
