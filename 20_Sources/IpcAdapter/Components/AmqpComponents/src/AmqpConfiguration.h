#pragma once

#include <QHostAddress>
#include <QString>



namespace IpcAdapter
{
    namespace Components
    {
        namespace AmqpComponents
        {
            struct AmqpConfiguration
            {
                QHostAddress host;
                quint16 port = 0;

                QString user;
                QString pwd;
                QString vhost;

                QString exchangeName;
                QString exchangeType;

                QString routingKey;

                bool isOk = false;


                AmqpConfiguration();

                void initDefaults();

                enum class UriStyle : bool
                {
                    ForUsage,
                    LogSafe
                };

                QString getAmqpUri(UriStyle const anUriStyle) const;
            };
        }
    }
}
