#pragma execution_character_set("utf-8")

#define TIMEMS      qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))

#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->pushButton_3->setDisabled(true);
	ui->pushButton_4->setDisabled(true);

	//setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
	//setFixedSize(this->width(), this->height());                     // 禁止拖动窗口大小



	customMsgBox.setWindowTitle(tr("关于本软件"));
	customMsgBox.addButton(tr("好的"), QMessageBox::ActionRole);
	customMsgBox.setIconPixmap(QPixmap(":/myImage/images/about1.png"));
	customMsgBox.setText(tr("欢迎使用《多功能图像视频处理》软件！本软件具有简单的图像和视频处理功能。\n"
		"图像功能包括多选打开、旋转、镜像、灰度化、均值滤波、"
		"边缘检测、原图复合、伽马检测、二值化、色彩调整、亮度调整、对比度调整、饱和度调整等功能。\n"
		"视频功能包括暂停、播放、进度条、灰度化、边缘检测、平滑、二值化、局部马赛克、缩放等功能。\n"
		"——By fyf"));

	ui->statusBar->showMessage(tr("欢迎使用多功能浏览器"), 2000);
	QLabel* permanent = new QLabel(this);
	permanent->setObjectName("status");
	permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);
	permanent->setText("欢迎使用！");
	ui->statusBar->addPermanentWidget(permanent);
	ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px;background:  transparent; }");
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_action_Open_triggered()
{
	QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "./images", tr("图像文件(*.jpg *.png *.bmp)"));
	if (srcDirPathListS.size() > 0)
	{
		ui->tabWidget->setCurrentIndex(0);
	}
	if (srcDirPathListS.size() >= 3) {
		srcDirPathList = srcDirPathListS;
		srcDirPathListS.clear();
		index = 0;
		QString srcDirPath = srcDirPathList.at(index);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);

		QString src1 = srcDirPathList.at((index + 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);

		QString src2 = srcDirPathList.at((index + 2) % srcDirPathList.size());
		QImage image2(src2);
		QImage Image2 = ImageCenter(image2, ui->label_other_3);
		ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
		ui->label_other_3->setAlignment(Qt::AlignCenter);
		ui->pushButton_3->setDisabled(false);
		ui->pushButton_4->setDisabled(false);
		ui->label_other_1->setVisible(true);
		ui->label_other_3->setVisible(true);
	}
	else if (srcDirPathListS.size() == 1) {
		srcDirPathList = srcDirPathListS;
		srcDirPathListS.clear();
		index = 0;
		QString srcDirPath = srcDirPathList.at(index);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);
		//有图片触发事件
		//isImage=true;
		//qDebug("%d",srcDirPathList.size());
		ui->pushButton_3->setDisabled(true);
		ui->pushButton_4->setDisabled(true);
		ui->label_other_3->setVisible(false);
		ui->label_other_1->setVisible(false);
	}
	else if (srcDirPathListS.size() == 2) {
		srcDirPathList = srcDirPathListS;
		srcDirPathListS.clear();
		index = 0;
		QString srcDirPath = srcDirPathList.at(index);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);
		//有图片触发事件
		//isImage=true;
		//qDebug("%d",srcDirPathList.size());
		QString src1 = srcDirPathList.at((index + 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);
		ui->pushButton_3->setDisabled(false);
		ui->pushButton_4->setDisabled(false);
		ui->label_other_1->setVisible(true);
		ui->label_other_3->setVisible(false);
	}
}

//图片居中显示,图片大小与label大小相适应
QImage MainWindow::ImageCenter(QImage  qimage, QLabel* qLabel)
{
	QImage image;
	QSize imageSize = qimage.size();
	QSize labelSize = qLabel->size();

	double dWidthRatio = 1.0 * imageSize.width() / labelSize.width();
	double dHeightRatio = 1.0 * imageSize.height() / labelSize.height();
	if (dWidthRatio > dHeightRatio)
	{
		image = qimage.scaledToWidth(labelSize.width());
	}
	else
	{
		image = qimage.scaledToHeight(labelSize.height());
	}
	return image;

}

//选择图片
void MainWindow::on_pushButton_clicked()
{
	QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "./images", tr("图像文件(*.jpg *.png *.bmp)"));
	if (srcDirPathListS.size() > 0) {
		ui->tabWidget->setCurrentIndex(0);
	}
	if (srcDirPathListS.size() >= 3) {
		srcDirPathList = srcDirPathListS;
		srcDirPathListS.clear();
		index = 0;
		QString srcDirPath = srcDirPathList.at(index);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);

		QString src1 = srcDirPathList.at((index + 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);

		QString src2 = srcDirPathList.at((index + 2) % srcDirPathList.size());
		QImage image2(src2);
		QImage Image2 = ImageCenter(image2, ui->label_other_3);
		ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
		ui->label_other_3->setAlignment(Qt::AlignCenter);
		ui->pushButton_3->setDisabled(false);
		ui->pushButton_4->setDisabled(false);
		ui->label_other_1->setVisible(true);
		ui->label_other_3->setVisible(true);
	}
	else if (srcDirPathListS.size() == 1) {
		srcDirPathList = srcDirPathListS;
		srcDirPathListS.clear();
		index = 0;
		QString srcDirPath = srcDirPathList.at(index);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);
		//有图片触发事件
		//isImage=true;
		//qDebug("%d",srcDirPathList.size());
		ui->pushButton_3->setDisabled(true);
		ui->pushButton_4->setDisabled(true);
		ui->label_other_3->setVisible(false);
		ui->label_other_1->setVisible(false);
	}
	else if (srcDirPathListS.size() == 2) {
		srcDirPathList = srcDirPathListS;
		srcDirPathListS.clear();
		index = 0;
		QString srcDirPath = srcDirPathList.at(index);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);
		//有图片触发事件
		//isImage=true;
		//qDebug("%d",srcDirPathList.size());
		QString src1 = srcDirPathList.at((index + 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);
		ui->pushButton_3->setDisabled(false);
		ui->pushButton_4->setDisabled(false);
		ui->label_other_1->setVisible(true);
		ui->label_other_3->setVisible(false);
	}
}

//上一张
void MainWindow::on_pushButton_3_clicked()
{
	if (srcDirPathList.size() >= 3) {
		index = qAbs(index + srcDirPathList.size() - 1);
		int i = index % srcDirPathList.size();
		//qDebug("%d",i);
		QString srcDirPath = srcDirPathList.at(i);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images3 = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images3));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);

		QString src1 = srcDirPathList.at(qAbs(index + srcDirPathList.size() - 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);

		QString src2 = srcDirPathList.at(qAbs(index + srcDirPathList.size() - 2) % srcDirPathList.size());
		QImage image2(src2);
		QImage Image2 = ImageCenter(image2, ui->label_other_3);
		ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
		ui->label_other_3->setAlignment(Qt::AlignCenter);
	}
	else if (srcDirPathList.size() == 2) {
		index = qAbs(index + srcDirPathList.size() - 1);
		int i = index % srcDirPathList.size();
		//qDebug("%d",i);
		QString srcDirPath = srcDirPathList.at(i);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images3 = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images3));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);

		QString src1 = srcDirPathList.at(qAbs(index + srcDirPathList.size() - 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);
	}
}

//下一张
void MainWindow::on_pushButton_4_clicked()
{

	if (srcDirPathList.size() >= 3) {
		index = qAbs(index + 1);
		int i = index % srcDirPathList.size();
		// qDebug("%d",i);
		QString srcDirPath = srcDirPathList.at(i);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images1 = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images1));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);


		QString src1 = srcDirPathList.at((index + 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);

		QString src2 = srcDirPathList.at((index + 2) % srcDirPathList.size());
		QImage image2(src2);
		QImage Image2 = ImageCenter(image2, ui->label_other_3);
		ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
		ui->label_other_3->setAlignment(Qt::AlignCenter);
	}
	else if (srcDirPathList.size() == 2) {
		index = qAbs(index + 1);
		int i = index % srcDirPathList.size();
		QString srcDirPath = srcDirPathList.at(i);
		QImage image(srcDirPath);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		origin_path = srcDirPath;
		QImage images1 = ImageCenter(image, ui->label_other);
		ui->label_other->setPixmap(QPixmap::fromImage(images1));
		ui->label_other->setAlignment(Qt::AlignCenter);
		//状态栏显示图片路径
		QLabel* label = ui->statusBar->findChild<QLabel*>("status");
		label->setText(srcDirPath);

		QString src1 = srcDirPathList.at((index + 1) % srcDirPathList.size());
		QImage image1(src1);
		QImage Image1 = ImageCenter(image1, ui->label_other_1);
		ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
		ui->label_other_1->setAlignment(Qt::AlignCenter);
	}
}

//保存
void MainWindow::on_action_Save_triggered()
{
	if (ui->checkBox->isChecked()) {//要加水印

		if (ui->label_show->pixmap() != nullptr) {

			QImage image2(ui->label_show->pixmap()->toImage());
			QImage simage("./images/name1.png");

			int swidth = simage.width();
			int sheight = simage.height();
			int r, b, g;

			for (int i = 0; i < sheight; ++i) {
				for (int j = 0; j < swidth; ++j) {
					QColor oldcolor2 = QColor(simage.pixel(j, i));
					r = oldcolor2.red();
					b = oldcolor2.blue();
					g = oldcolor2.green();

					if (r == 0 && b == 0 && g == 0)
					{
						image2.setPixelColor(j, i, qRgb(0, 0, 0));
					}
					else
					{
						//image.setPixelColor(j,i,qRgb(red,blue,green));
					}
				}

			}


			QString filename = QFileDialog::getSaveFileName(this,
				tr("保存图片"),
				"./images/signed_images.png",
				tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
			if (filename.isEmpty())
			{
				return;
			}
			else
			{
				if (!(image2.save(filename))) //保存图像
				{

					QMessageBox::information(this,
						tr("图片保存成功！"),
						tr("图片保存失败！"));
					return;
				}
				ui->statusBar->showMessage("图片保存成功！");
			}

		}
		else {
			QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
		}

	}
	else //不加水印
	{
		if (ui->label_show->pixmap() != nullptr) {
			QString filename = QFileDialog::getSaveFileName(this,
				tr("保存图片"),
				"./images/images.png",
				tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
			if (filename.isEmpty())
			{
				return;
			}
			else
			{
				if (!(ui->label_show->pixmap()->toImage().save(filename))) //保存图像
				{

					QMessageBox::information(this,
						tr("图片保存成功！"),
						tr("图片保存失败！"));
					return;
				}
				ui->statusBar->showMessage("图片保存成功！");
			}

		}
		else {
			QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
		}
	}
}

//灰度化
QImage MainWindow::gray(QImage image) {
	QImage newImage = image.convertToFormat(QImage::Format_ARGB32);
	QColor oldColor;

	for (int y = 0; y < newImage.height(); y++)
	{
		for (int x = 0; x < newImage.width(); x++)
		{
			oldColor = QColor(image.pixel(x, y));
			int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
			newImage.setPixel(x, y, qRgb(average, average, average));
		}
	}
	return newImage;
}

//灰度化
void MainWindow::on_pushButton_gray_clicked()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);

		QImage images = gray(image);

		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//均值滤波
QImage MainWindow::AverageFiltering(QImage image) {
	int kernel[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1} };
	int sizeKernel = 3;
	int sumKernel = 9;
	QColor color;
	for (int x = sizeKernel / 2; x < image.width() - sizeKernel / 2; x++)
	{
		for (int y = sizeKernel / 2; y < image.height() - sizeKernel / 2; y++)
		{
			int r = 0;
			int g = 0;
			int b = 0;
			for (int i = -sizeKernel / 2; i <= sizeKernel / 2; i++)
			{
				for (int j = -sizeKernel / 2; j <= sizeKernel / 2; j++)
				{
					color = QColor(image.pixel(x + i, y + j));
					r += color.red() * kernel[sizeKernel / 2 + i][sizeKernel / 2 + j];
					g += color.green() * kernel[sizeKernel / 2 + i][sizeKernel / 2 + j];
					b += color.blue() * kernel[sizeKernel / 2 + i][sizeKernel / 2 + j];
				}
			}
			r = qBound(0, r / sumKernel, 255);
			g = qBound(0, g / sumKernel, 255);
			b = qBound(0, b / sumKernel, 255);
			image.setPixel(x, y, qRgb(r, g, b));
		}
	}
	return image;
}

//均值滤波
void MainWindow::on_pushButton_junzhi_clicked()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		image = AverageFiltering(image);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//翻译
void MainWindow::on_action_L_triggered()
{
	QTranslator translator;
	if (language) {
		translator.load("zh_tr.qm");
	}
	else
	{
		translator.load("en_tr.qm");
	}
	QApplication* qapp;
	qapp->installTranslator(&translator);
	language = !language;
	ui->retranslateUi(this);//重新翻译刷新界面
}

//亮度调节
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		int red, green, blue;
		int pixels = image.width() * image.height();
		unsigned int* data = (unsigned int*)image.bits();

		for (int i = 0; i < pixels; ++i)
		{
			red = qRed(data[i]) + value;
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = qGreen(data[i]) + value;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = qBlue(data[i]) + value;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
			data[i] = qRgba(red, green, blue, qAlpha(data[i]));
		}
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		ui->label_light->setText(QString::number(value));
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}

}

//边缘检测
QImage MainWindow::EdgeFliter(QImage image) {
	QImage newImage = image.convertToFormat(QImage::Format_ARGB32);
	QColor color0;
	QColor color1;
	QColor color2;
	QColor color3;
	int  r = 0;
	int g = 0;
	int b = 0;
	int rgb = 0;
	int r1 = 0;
	int g1 = 0;
	int b1 = 0;
	int rgb1 = 0;
	int a = 0;
	for (int y = 0; y < image.height() - 1; y++)
	{
		for (int x = 0; x < image.width() - 1; x++)
		{
			color0 = QColor(image.pixel(x, y));
			color1 = QColor(image.pixel(x + 1, y));
			color2 = QColor(image.pixel(x, y + 1));
			color3 = QColor(image.pixel(x + 1, y + 1));
			r = abs(color0.red() - color3.red());
			g = abs(color0.green() - color3.green());
			b = abs(color0.blue() - color3.blue());
			rgb = r + g + b;

			r1 = abs(color1.red() - color2.red());
			g1 = abs(color1.green() - color2.green());
			b1 = abs(color1.blue() - color2.blue());
			rgb1 = r1 + g1 + b1;

			a = rgb + rgb1;
			a = a > 255 ? 255 : a;

			newImage.setPixel(x, y, qRgb(a, a, a));
		}
	}
	return newImage;
}

//均值滤波
void MainWindow::on_pushButton_junzhi_2_clicked()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		QImage newImage = EdgeFliter(image);
		QImage Image = ImageCenter(newImage, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//原图+边缘检测复合调用函数
QImage MainWindow::OriginalPlusEdgeFliter(QImage images) {
	QImage image2 = images.convertToFormat(QImage::Format_ARGB32);
	QColor color0;
	QColor color1;
	QColor color2;
	QColor color3;
	int  r = 0;
	int g = 0;
	int b = 0;
	int rgb = 0;
	int r1 = 0;
	int g1 = 0;
	int b1 = 0;
	int rgb1 = 0;
	int a = 0;
	for (int y = 0; y < images.height() - 1; y++)
	{
		for (int x = 0; x < images.width() - 1; x++)
		{
			color0 = QColor(images.pixel(x, y));
			color1 = QColor(images.pixel(x + 1, y));
			color2 = QColor(images.pixel(x, y + 1));
			color3 = QColor(images.pixel(x + 1, y + 1));
			r = abs(color0.red() - color3.red());
			g = abs(color0.green() - color3.green());
			b = abs(color0.blue() - color3.blue());
			rgb = r + g + b;

			r1 = abs(color1.red() - color2.red());
			g1 = abs(color1.green() - color2.green());
			b1 = abs(color1.blue() - color2.blue());
			rgb1 = r1 + g1 + b1;

			a = rgb + rgb1;
			a = a > 255 ? 255 : a;

			image2.setPixel(x, y, qRgb(a, a, a));
		}
	}
	QImage image(origin_path);
	int red, green, blue;
	int red2, green2, blue2;
	int pixels = image.width() * image.height();

	unsigned int* data = (unsigned int*)image.bits();
	unsigned int* data2 = (unsigned int*)image2.bits();

	for (int i = 0; i < pixels; ++i)
	{
		red = qRed(data[i]);
		red2 = qRed(data2[i]) * 0.5 + red * 0.5;
		red2 = (red2 < 0x00) ? 0x00 : (red2 > 0xff) ? 0xff : red2;

		green = qGreen(data[i]);
		green2 = qGreen(data2[i]) * 0.5 + green * 0.5;
		green2 = (green2 < 0x00) ? 0x00 : (green2 > 0xff) ? 0xff : green2;

		blue = qBlue(data[i]);
		blue2 = qBlue(data2[i]) * 0.5 + blue * 0.5;
		blue2 = (blue2 < 0x00) ? 0x00 : (blue2 > 0xff) ? 0xff : blue2;

		data2[i] = qRgba(red2, green2, blue2, qAlpha(data2[i]));
	}
	return image2;
}

//边缘检测+原图复合
void MainWindow::on_pushButton_junzhi_3_clicked()
{

	if (origin_path != nullptr) {
		QImage images(origin_path);
		QImage image2 = OriginalPlusEdgeFliter(images);
		QImage Image = ImageCenter(image2, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//更改比例
void MainWindow::on_horizontalSlider_2_valueChanged(int value1)
{
	float value = (float)value1 / 100;
	if (origin_path != nullptr) {
		QImage images(origin_path);
		QImage image2 = images.convertToFormat(QImage::Format_ARGB32);
		QColor color0;
		QColor color1;
		QColor color2;
		QColor color3;
		int  r = 0;
		int g = 0;
		int b = 0;
		int rgb = 0;
		int r1 = 0;
		int g1 = 0;
		int b1 = 0;
		int rgb1 = 0;
		int a = 0;
		for (int y = 0; y < images.height() - 1; y++)
		{
			for (int x = 0; x < images.width() - 1; x++)
			{
				color0 = QColor(images.pixel(x, y));
				color1 = QColor(images.pixel(x + 1, y));
				color2 = QColor(images.pixel(x, y + 1));
				color3 = QColor(images.pixel(x + 1, y + 1));
				r = abs(color0.red() - color3.red());
				g = abs(color0.green() - color3.green());
				b = abs(color0.blue() - color3.blue());
				rgb = r + g + b;

				r1 = abs(color1.red() - color2.red());
				g1 = abs(color1.green() - color2.green());
				b1 = abs(color1.blue() - color2.blue());
				rgb1 = r1 + g1 + b1;

				a = rgb + rgb1;
				a = a > 255 ? 255 : a;

				image2.setPixel(x, y, qRgb(a, a, a));
			}
		}
		QImage image(origin_path);
		//    QImage image2(label->pixmap()->toImage());
		int red, green, blue;
		int red2, green2, blue2;
		int pixels = image.width() * image.height();

		unsigned int* data = (unsigned int*)image.bits();
		unsigned int* data2 = (unsigned int*)image2.bits();

		for (int i = 0; i < pixels; ++i)
		{
			red = qRed(data[i]);
			red2 = qRed(data2[i]) * value + red * (1 - value);
			red2 = (red2 < 0x00) ? 0x00 : (red2 > 0xff) ? 0xff : red2;

			green = qGreen(data[i]);
			green2 = qGreen(data2[i]) * value + green * (1 - value);
			green2 = (green2 < 0x00) ? 0x00 : (green2 > 0xff) ? 0xff : green2;

			blue = qBlue(data[i]);
			blue2 = qBlue(data2[i]) * value + blue * (1 - value);
			blue2 = (blue2 < 0x00) ? 0x00 : (blue2 > 0xff) ? 0xff : blue2;

			data2[i] = qRgba(red2, green2, blue2, qAlpha(data2[i]));
		}
		QImage Image = ImageCenter(image2, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		ui->label_fuhe->setText(QString::number(value).append(":").append(QString::number(1 - value)));
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//保存
void MainWindow::on_pushButton_save_clicked()
{
	if (ui->checkBox->isChecked()) {//要加水印

		if (ui->label_show->pixmap() != nullptr) {

			QImage image2(ui->label_show->pixmap()->toImage());
			QImage simage(":/images/name1.png");

			int swidth = simage.width();
			int sheight = simage.height();
			int r, b, g;

			for (int i = 0; i < sheight; ++i) {
				for (int j = 0; j < swidth; ++j) {
					QColor oldcolor2 = QColor(simage.pixel(j, i));
					r = oldcolor2.red();
					b = oldcolor2.blue();
					g = oldcolor2.green();

					if (r == 0 && b == 0 && g == 0)
					{
						image2.setPixelColor(j, i, qRgb(0, 0, 0));
					}
					else
					{
						//image.setPixelColor(j,i,qRgb(red,blue,green));
					}
				}

			}


			QString filename = QFileDialog::getSaveFileName(this,
				tr("保存图片"),
				":/images/signed_images.png",
				tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
			if (filename.isEmpty())
			{
				return;
			}
			else
			{
				if (!(image2.save(filename))) //保存图像
				{

					QMessageBox::information(this,
						tr("图片保存成功！"),
						tr("图片保存失败！"));
					return;
				}
				ui->statusBar->showMessage("图片保存成功！");
			}

		}
		else {
			QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
		}

	}
	else //不加水印
	{
		if (ui->label_show->pixmap() != nullptr) {
			QString filename = QFileDialog::getSaveFileName(this,
				tr("保存图片"),
				":/images",
				tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
			if (filename.isEmpty())
			{
				return;
			}
			else
			{
				if (!(ui->label_show->pixmap()->toImage().save(filename))) //保存图像
				{

					QMessageBox::information(this,
						tr("图片保存成功！"),
						tr("图片保存失败！"));
					return;
				}
				ui->statusBar->showMessage("图片保存成功！");
			}

		}
		else {
			QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
		}
	}
}

//改变R值滑动条
void MainWindow::on_horizontalSlider_R_valueChanged(int value)
{
	if (ui->label_show->pixmap() != nullptr) {
		int value_r = value;
		int value_g = ui->horizontalSlider_G->value();
		int value_b = ui->horizontalSlider_B->value();
		QImage image(origin_path);
		QImage images = setRGB(image, value_r, value_g, value_b);
		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
	}


}

//调整rgb函数调用
QImage MainWindow::setRGB(QImage image, int value_r, int value_g, int value_b) {
	int r, g, b;
	QColor oldColor;
	int height = image.height();
	int width = image.width();
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j) {
			oldColor = QColor(image.pixel(j, i));
			r = oldColor.red() + value_r;
			if (r > 255)
				r = 255;
			g = oldColor.green() + value_g;
			if (g > 255)
				g = 255;
			b = oldColor.blue() + value_b;
			if (b > 255)
				b = 255;
			image.setPixel(j, i, qRgb(r, g, b));
		}
	}
	return image;
}

//显示原图按钮
void MainWindow::on_pushButton_origin_clicked()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//改变g值滑动条
void MainWindow::on_horizontalSlider_G_valueChanged(int value)
{
	if (ui->label_show->pixmap() != nullptr) {
		int value_r = ui->horizontalSlider_R->value();
		int value_g = value;
		int value_b = ui->horizontalSlider_B->value();
		QImage image(origin_path);
		QImage images = setRGB(image, value_r, value_g, value_b);
		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//改变b值滑动条
void MainWindow::on_horizontalSlider_B_valueChanged(int value)
{
	if (ui->label_show->pixmap() != nullptr) {
		int value_r = ui->horizontalSlider_R->value();
		int value_g = ui->horizontalSlider_G->value();
		int value_b = value;
		QImage image(origin_path);
		QImage images = setRGB(image, value_r, value_g, value_b);
		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

QImage MainWindow::gamma(QImage image) {
	double d = 1.2;
	QColor color;
	int height = image.height();
	int width = image.width();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			color = QColor(image.pixel(i, j));
			double r = color.red();
			double g = color.green();
			double b = color.blue();
			int R = qBound(0, (int)qPow(r, d), 255);
			int G = qBound(0, (int)qPow(g, d), 255);
			int B = qBound(0, (int)qPow(b, d), 255);
			image.setPixel(i, j, qRgb(R, G, B));
		}
	}
	return image;
}

//伽马变换按钮
void MainWindow::on_pushButton_gamma_clicked()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		image = gamma(image);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
	}

}

//二值化滑动条
void MainWindow::on_horizontalSlider_erzhi_valueChanged(int value)
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		QImage images = gray(image);
		int height = images.height();
		int width = images.width();
		int bt;
		QColor oldColor;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j) {
				oldColor = QColor(images.pixel(j, i));
				bt = oldColor.red();
				if (bt < value) {
					bt = 0;
				}
				else {
					bt = 255;
				}
				images.setPixel(j, i, qRgb(bt, bt, bt));

			}
		}
		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
		ui->label_yuzhi->setText(QString::number(value));
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//调整对比度
QImage MainWindow::AdjustContrast(QImage image, int value)
{
	int pixels = image.width() * image.height();
	unsigned int* data = (unsigned int*)image.bits();

	int red, green, blue, nRed, nGreen, nBlue;

	if (value > 0 && value < 256)
	{
		float param = 1 / (1 - value / 256.0) - 1;

		for (int i = 0; i < pixels; ++i)
		{
			nRed = qRed(data[i]);
			nGreen = qGreen(data[i]);
			nBlue = qBlue(data[i]);

			red = nRed + (nRed - 127) * param;
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = nGreen + (nGreen - 127) * param;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = nBlue + (nBlue - 127) * param;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

			data[i] = qRgba(red, green, blue, qAlpha(data[i]));
		}
	}
	else
	{
		for (int i = 0; i < pixels; ++i)
		{
			nRed = qRed(data[i]);
			nGreen = qGreen(data[i]);
			nBlue = qBlue(data[i]);

			red = nRed + (nRed - 127) * value / 100.0;
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = nGreen + (nGreen - 127) * value / 100.0;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = nBlue + (nBlue - 127) * value / 100.0;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

			data[i] = qRgba(red, green, blue, qAlpha(data[i]));
		}
	}

	return image;
}

//对比度滑动条
void MainWindow::on_horizontalSlider_duibi_valueChanged(int value)
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		QImage images = AdjustContrast(image, value);
		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//饱和度函数调用
QImage MainWindow::AdjustSaturation(QImage Img, int iSaturateValue)
{
	int red, green, blue, nRed, nGreen, nBlue;
	int pixels = Img.width() * Img.height();
	unsigned int* data = (unsigned int*)Img.bits();

	float Increment = iSaturateValue / 100.0;

	float delta = 0;
	float minVal, maxVal;
	float L, S;
	float alpha;

	for (int i = 0; i < pixels; ++i)
	{
		nRed = qRed(data[i]);
		nGreen = qGreen(data[i]);
		nBlue = qBlue(data[i]);

		minVal = std::min(std::min(nRed, nGreen), nBlue);
		maxVal = std::max(std::max(nRed, nGreen), nBlue);
		delta = (maxVal - minVal) / 255.0;
		L = 0.5 * (maxVal + minVal) / 255.0;
		S = std::max(0.5 * delta / L, 0.5 * delta / (1 - L));

		if (Increment > 0)
		{
			alpha = std::max(S, 1 - Increment);
			alpha = 1.0 / alpha - 1;
			red = nRed + (nRed - L * 255.0) * alpha;
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = nGreen + (nGreen - L * 255.0) * alpha;
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = nBlue + (nBlue - L * 255.0) * alpha;
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
		}
		else
		{
			alpha = Increment;
			red = L * 255.0 + (nRed - L * 255.0) * (1 + alpha);
			red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
			green = L * 255.0 + (nGreen - L * 255.0) * (1 + alpha);
			green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
			blue = L * 255.0 + (nBlue - L * 255.0) * (1 + alpha);
			blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
		}

		data[i] = qRgba(red, green, blue, qAlpha(data[i]));
	}

	return Img;
}

//饱和度
void MainWindow::on_horizontalSlider_baohe_valueChanged(int value)
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		QImage images = AdjustSaturation(image, value);
		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//打开视频
void MainWindow::on_action_V_triggered()
{
	video_path = QFileDialog::getOpenFileName(this, tr("选择视频"), "./images", tr("Video (*.WMV *.mp4 *.rmvb *.flv)"));

	if (video_path.isEmpty())
	{
		return;
	}

	ui->tabWidget->setCurrentIndex(1);
	ui->pushButton_6->setEnabled(true);

	if (ffmpeg != nullptr) {
		ffmpeg->Stop();
	}

	ffmpeg = new XFFmpeg(this);
	ffmpeg->Abort = false;
	
	connect(ffmpeg, SIGNAL(receiveTotalVideoTime(double)), this, SLOT(onGetTotalVideoTime(double)));
	connect(ffmpeg, SIGNAL(receiveCurrentVideoTime(double)), this, SLOT(onGetCurrentVideoTime(double)));
	connect(ffmpeg, SIGNAL(receiveImage(QImage)), this, SLOT(onDisplayImage(QImage)));
	connect(this, SIGNAL(receiveIsPlay(bool)), ffmpeg, SLOT(onGetIsPlay(bool)));
	connect(ffmpeg, SIGNAL(finished()), this, SLOT(deleteLater()));

	ffmpeg->Open(video_path);

	type = 0;                                               //默认打开不进行处理
	isPlay = true;
	ui->pushButton_6->setStyleSheet(
		"border-image:url(:/player/icon/player/play.png);"
		"background-color:transparent;"
		"border:none");

	ffmpeg->start();
}

//ffmpeg->start()响应
void MainWindow::onDisplayImage(const QImage& image)
{
	cv::Mat cv_frame = QImage2cvMat(image);

	if (type == 1) {
		//image=gray2(image);
		cvtColor(cv_frame, cv_frame, CV_BGR2GRAY);
		qDebug() << TIMEMS << "type1";
	}
	else if (type == 2) {
		cvtColor(cv_frame, cv_frame, CV_BGR2GRAY);
		//高斯滤波
		GaussianBlur(cv_frame, cv_frame, Size(3, 3),
			0, 0, BORDER_DEFAULT);
		//Canny检测
		int edgeThresh = 100;
		Mat Canny_result;
		Canny(cv_frame, cv_frame, edgeThresh, edgeThresh * 3, 3);
		qDebug() << TIMEMS << "type2";

	}
	else if (type == 3) {
		//Smooth(frame, frame,Size(3, 3), 0, 0);
		GaussianBlur(cv_frame, cv_frame, Size(3, 3), 0, 0);
		qDebug() << TIMEMS << "type3";
	}
	else if (type == 4) {
		cvtColor(cv_frame, cv_frame, CV_BGR2GRAY);
		threshold(cv_frame, cv_frame, 96, 255, THRESH_BINARY);
		qDebug() << TIMEMS << "type4";
	}
	else if (type == 5) {
		cv_frame = Mosaic(cv_frame);
		qDebug() << TIMEMS << "type5";
	}

	//opencv: Mat转Qimage
	QImage imageDisplay = Mat2QImage(cv_frame);

	//绘制到指定位置
	double scale = ui->horizontalSlider_suofang->value() / 100.0;
	QImage Image = ImageCenter(imageDisplay, ui->label_11);
	QSize qs = Image.size() * scale;

	ui->label_11->setPixmap(QPixmap::fromImage(Image).scaled(qs, Qt::KeepAspectRatio));
	ui->label_11->setAlignment(Qt::AlignCenter);
	ui->label_11->repaint();

	//进度位置
	ui->VideohorizontalSlider_2->setValue(currentVideoTime);
	ui->VideohorizontalSlider_2->setMaximum(totalVideoTime);

	//lable_l2播放时间显示
	ui->label_12->setText(stom((long)currentVideoTime) + "/" + stom((long)totalVideoTime));
}

void MainWindow::onGetCurrentVideoTime(const double& time)
{
	this->currentVideoTime = time;
}

void MainWindow::onGetTotalVideoTime(const double& time)
{
	this->totalVideoTime = time;
}

cv::Mat MainWindow::QImage2cvMat(QImage image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}

//Mat转图像
QImage MainWindow::Mat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1) {
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat
		const uchar* pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

//Avframe*转为openCV中的Mat
cv::Mat MainWindow::Avframe2cvMat(AVFrame* avframe, int w, int h)
{
	int width = avframe->width;
	int height = avframe->height;
	cv::Mat image(height, width, CV_8UC3);
	int cvLinesizes[1];
	cvLinesizes[0] = image.step1();
	SwsContext* conversion = sws_getContext(width, height, (AVPixelFormat)avframe->format, width, height, AVPixelFormat::AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	sws_scale(conversion, avframe->data, avframe->linesize, 0, height, &image.data, cvLinesizes);
	sws_freeContext(conversion);
	return image;
}

//秒转分函数
QString MainWindow::stom(int s) {
	QString m;
	if (s / 60 == 0) {
		m = QString::number(s % 60);
	}
	else {
		m = QString::number(s / 60) + ":" + QString::number(s % 60);
	}
	return m;
}

//暂停/播放
void MainWindow::on_pushButton_6_clicked()
{
	if (isPlay)
	{
		isPlay = false;
		ui->pushButton_6->setStyleSheet(
			"border-image:url(:/player/icon/player/pause.png);"
			"background-color:transparent;"
			"border:none");
	}
	else {
		isPlay = true;
		ui->pushButton_6->setStyleSheet("border-image:url(:/player/icon/player/play.png);"
			"background-color:transparent;"
			"border:none");
	}
	emit receiveIsPlay(isPlay);
}

//灰度
void MainWindow::on_pushButton_7_clicked()
{
	type = 1;
}

//边缘检测
void MainWindow::on_pushButton_8_clicked()
{
	type = 0;
}

//均值滤波
void MainWindow::on_pushButton_9_clicked()
{
	type = 2;
}

//平滑
void MainWindow::on_pushButton_10_clicked()
{
	type = 3;
}

//二值化
void MainWindow::on_pushButton_11_clicked()
{
	type = 4;
}

//缩放
void MainWindow::on_horizontalSlider_suofang_valueChanged(int value)
{

	ui->label_suofangvalue->setText(QString::number(value / 100.0));
	cout << value << endl;
}

Mat MainWindow::Mosaic(Mat src) {
	int width = src.rows;	//图片的长度
	int height = src.cols;	//图片的宽度

	//10*10的像素点进行填充
	int arr = 10;

	//i和j代表了矩形区域的左上角的像素坐标
	for (int i = width / 2.5; i < width / 1.5; i += arr) {
		for (int j = height / 2.5; j < height / 1.5; j += arr) {
			//对矩形区域内的每一个像素值进行遍历
			for (int k = i; k < arr + i && k < width; k++) {
				for (int m = j; m < arr + j && m < height; m++) {
					//在这里进行颜色的修改
					src.at<Vec3b>(k, m)[0] = src.at<Vec3b>(i, j)[0];
					src.at<Vec3b>(k, m)[1] = src.at<Vec3b>(i, j)[1];
					src.at<Vec3b>(k, m)[2] = src.at<Vec3b>(i, j)[2];
				}
			}
		}
	}
	return src;
}

//马赛克
void MainWindow::on_pushButton_2_clicked()
{
	type = 5;
}

//工具栏灰度化
void MainWindow::on_action_H_triggered()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);

		QImage images = gray(image);

		QImage Image = ImageCenter(images, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//工具栏均值滤波
void MainWindow::on_action_J_triggered()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		image = AverageFiltering(image);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//工具栏边缘检测
void MainWindow::on_action_B_triggered()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		QImage newImage = EdgeFliter(image);
		QImage Image = ImageCenter(newImage, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//工具栏伽马变换
void MainWindow::on_action_G_triggered()
{
	if (origin_path != nullptr) {
		QImage image(origin_path);
		image = gamma(image);
		QImage Image = ImageCenter(image, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//工具栏边缘原图复合
void MainWindow::on_action_Y_triggered()
{
	if (origin_path != nullptr) {
		QImage images(origin_path);
		QImage image2 = OriginalPlusEdgeFliter(images);
		QImage Image = ImageCenter(image2, ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(Image));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

void MainWindow::on_action_About_triggered()
{
	customMsgBox.show();

	customMsgBox.exec();
}

//左转
void MainWindow::on_pushButton_turnleft_clicked()
{
	if (ui->label_show->pixmap() != nullptr) {
		QImage images(ui->label_show->pixmap()->toImage());
		QMatrix matrix;
		matrix.rotate(-90.0);//逆时针旋转90度
		images = images.transformed(matrix, Qt::FastTransformation);
		//QImage Image=ImageCenter(images,ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(images));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

void MainWindow::on_pushButton_turnright_clicked()
{
	if (ui->label_show->pixmap() != nullptr) {
		QImage images(ui->label_show->pixmap()->toImage());
		QMatrix matrix;
		matrix.rotate(90.0);//逆时针旋转90度
		images = images.transformed(matrix, Qt::FastTransformation);
		//QImage Image=ImageCenter(images,ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(images));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

void MainWindow::on_pushButton_turnleft_2_clicked()
{
	if (ui->label_show->pixmap() != nullptr) {
		QImage images(ui->label_show->pixmap()->toImage());
		images = images.mirrored(true, false);
		//QImage Image=ImageCenter(images,ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(images));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

void MainWindow::on_pushButton_turnleft_3_clicked()
{
	if (ui->label_show->pixmap() != nullptr) {
		QImage images(ui->label_show->pixmap()->toImage());
		images = images.mirrored(false, true);
		//QImage Image=ImageCenter(images,ui->label_show);
		ui->label_show->setPixmap(QPixmap::fromImage(images));
		ui->label_show->setAlignment(Qt::AlignCenter);
	}
	else {
		QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes | QMessageBox::Yes);
	}
}

//背景处理
void MainWindow::on_pushButton_5_clicked()
{
	/*QFile file(":/myImage/images/style.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QString::fromLatin1(file.readAll());
	QApplication* qapp;
	qapp->setStyleSheet(styleSheet);*/

	if (video_path.isEmpty())
	{
		QMessageBox::warning(this, tr("警告"), tr("请打开视频后，重试"));
		return;
	}

	this->on_pushButton_6_clicked();
	ProcessBarWindow *pbw = new ProcessBarWindow(video_path);
	pbw->show();

	//ffmpeg->Play();

}