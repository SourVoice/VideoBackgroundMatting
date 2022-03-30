#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include "ui_ProcessBarWindow.h"
#include <qstring.h>
#include <cstdlib>
#include "BGProcess.h"
#include <filesystem>
using namespace std;
//using namespace std::tr2::sys;
using namespace std::filesystem;

class ProcessBarWindow : public QWidget
{
	Q_OBJECT

public:
	ProcessBarWindow(QString in_filepath, QWidget *parent = Q_NULLPTR);
	~ProcessBarWindow();
	void init();

private slots:
	void change_progress(const int value);
	void finished_operations();
	void on_finish_btn_clicked();

private:
	Ui::ProcessBarWindow ui;
	QString in_filepath;
	string out_filepath;
	BGProcess *process;
};
