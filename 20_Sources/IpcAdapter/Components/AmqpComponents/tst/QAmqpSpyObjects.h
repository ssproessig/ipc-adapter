#pragma once

#include <QString>

#include <memory>

#include "github.com.mbroadst/qamqp/qamqpclient.h"
#include "github.com.mbroadst/qamqp/qamqpexchange.h"



struct QAmqpExchangeSpy: QAmqpExchange
{
    void declare(const QString& type, ExchangeOptions, const QAmqpTable&) override
    {
        typeSeen = type;
    }

    void publish(const QString& message, const QString& routingKey, const QAmqpMessage::PropertyHash&, int) override
    {
        messageSeen = message;
        routingKeySeen = routingKey;
    }

    QString typeSeen;
    QString messageSeen;
    QString routingKeySeen;
};



struct QAmqpClientSpy: QAmqpClient
{
    void connectToHost(const QString& uri) override
    {
        uriSeen = uri;
    }

    QAmqpExchange* createExchange(const QString& name, int) override
    {
        exchangeNameSeen = name;
        exchange = std::make_shared<QAmqpExchangeSpy>();
        return exchange.get();
    }

    QString uriSeen;
    QString exchangeNameSeen;
    std::shared_ptr<QAmqpExchangeSpy> exchange;
};
