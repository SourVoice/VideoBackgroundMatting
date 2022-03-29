#include "ProcessBarWindow.h"

ProcessBarWindow::ProcessBarWindow(QString in_filepath, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowModality(Qt::ApplicationModal);//����Ϊģʽ�Ի���ͬʱ�ڹ���öԻ���ʱҪ���ø�����
	setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint&~Qt::WindowMinimizeButtonHint&~Qt::WindowMaximizeButtonHint);


	ui.progressbar->setStyleSheet("QProgressBar{background:white;border-radius:5px;} QProgressBar::chunk{border-radius:5px;background:blue}");
	ui.text_browser->setStyleSheet("QTextEdit{background-color:white;}");

	this->in_filepath = in_filepath;
	//path p(in_filepath.toStdString().c_str());
	//path filename = p.stem();
	//p.replace_filename(filename.string() + "_matting.mp4");
	QFileInfo info(in_filepath);
	QString strNewName = info.path() + "/" + info.completeBaseName() + "_matting.mp4";
	std::string str = std::string(strNewName.toLocal8Bit());
	this->out_filepath = str;

	init();

}


ProcessBarWindow::~ProcessBarWindow()
{
}

void ProcessBarWindow::init()
{
	ui.finish_btn->setEnabled(false);
	ui.finish_btn->setText(tr("�����У����Ժ�"));
	ui.input_video_path_label->setText(QString(this->in_filepath));
	ui.output_video_path_label->setText(QString(this->out_filepath.c_str()));
	process = new BGProcess(this->in_filepath.toStdString().c_str(), this->out_filepath);
	
	connect(process, SIGNAL(processed_frame_cnt(const int)), this, SLOT(change_progress(const int)));
	connect(process, &BGProcess::finished, this, &ProcessBarWindow::finished_operations);
	connect(process, &BGProcess::loading_modules, this, [=]() {
		ui.finish_btn->setText(tr("ģ�������"));
	});
	connect(process, &BGProcess::loading_finished, this, [=]() {
		ui.finish_btn->setText(tr("�����У����Ժ�"));
	});
	connect(process, &BGProcess::send_log, this, [=](const QString msg) {
		ui.text_browser->append(msg);
	});

	process->start();
}

void ProcessBarWindow::finished_operations()
{
	this->ui.progressbar->setValue(100);
	this->ui.finish_btn->setText(tr("������ɣ�����ر�"));
	this->ui.finish_btn->setEnabled(true);
	process->deleteLater();
}

void ProcessBarWindow::on_finish_btn_clicked()
{
	this->close();
}

void ProcessBarWindow::change_progress(const int value)
{
	this->ui.progressbar->setValue(value);
}
