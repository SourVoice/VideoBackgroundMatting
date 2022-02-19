#pragma once

#include <QObject>

class Worker : public QObject
{
	Q_OBJECT

public:
	Worker(QObject *parent);
	~Worker();
};
