#pragma once

#include <QDateTime>
#include <QDebug>



#define _LOG(instanceId) \
	QDateTime::currentSecsSinceEpoch() << "| " << static_cast<void*>(instanceId) << "|"

#define LOG_DEBUG(instanceId) \
	qDebug() << _LOG(instanceId)
