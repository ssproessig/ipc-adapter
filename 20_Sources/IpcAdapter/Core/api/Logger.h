#pragma once

#include <QDateTime>
#include <QDebug>



#define LOG_IMPL(instanceId) \
	QDateTime::currentSecsSinceEpoch() << "| " << static_cast<void*>(instanceId) << "|"

#define LOG_DEBUG(instanceId) \
	qDebug() << LOG_IMPL(instanceId)

#define LOG_ERROR(instanceId) \
	qCritical() << LOG_IMPL(instanceId)
