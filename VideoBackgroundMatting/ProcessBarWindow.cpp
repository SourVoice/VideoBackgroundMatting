#include "ProcessBarWindow.h"

ProcessBarWindow::ProcessBarWindow(QString in_filepath, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);//����Ϊģʽ�Ի���ͬʱ�ڹ���öԻ���ʱҪ���ø�����
	setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint&~Qt::WindowMinimizeButtonHint&~Qt::WindowMaximizeButtonHint);

	this->in_filepath = in_filepath;
	path p(in_filepath.toStdString().c_str());
	path filename = p.stem();
	p.replace_filename(filename.string() + "_matting.mp4");
	this->out_filepath = p.string();

	init();

}


ProcessBarWindow::~ProcessBarWindow()
{
}

void ProcessBarWindow::init()
{
	ui.finish_btn->setText(tr("�����У����Ժ�"));
	ui.input_video_path_label->setText(QString(this->in_filepath));
	ui.output_video_path_label->setText(QString(this->out_filepath.c_str()));
	process = new BGProcess(this->in_filepath.toStdString().c_str(), this->out_filepath);
	
	connect(process, SIGNAL(processed_frame_cnt(const int)), this, SLOT(change_progress(const int)));
	connect(process, &BGProcess::finished, this, &ProcessBarWindow::finished_operations);

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
