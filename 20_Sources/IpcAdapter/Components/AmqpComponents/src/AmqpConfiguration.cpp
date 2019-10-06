#include "AmqpConfiguration.h"

#include "Shared/api/ConvenienceMacros.h"



using IpcAdapter::Components::AmqpComponents::AmqpConfiguration;



AmqpConfiguration::AmqpConfiguration()
{
    initDefaults();
}



void AmqpConfiguration::initDefaults()
{
    REALIZE_REQUIREMENT("R-IPCA-AMQPCFG-002");

    host = QHostAddress("127.0.0.1");
    port = 5672;

    REALIZE_REQUIREMENT("R-IPCA-AMQPCFG-003");
    user = "guest";
    pwd = "guest";
    vhost = "/";

    REALIZE_REQUIREMENT("R-IPCA-AMQPCFG-004");
    exchangeName.clear();
    exchangeType = "direct";

    routingKey.clear();

    isOk = true;
}



QString AmqpConfiguration::getAmqpUri(UriStyle const anUriStyle) const
{
    REALIZE_REQUIREMENT("R-IPCA-AMQPCFG-005");

    auto const password = anUriStyle == UriStyle::ForUsage ? pwd : "***";

    return QStringLiteral("amqp://%1:%2@%3:%4%5")
           .arg(user).arg(password).arg(host.toString()).arg(port).arg(vhost.startsWith('/') ? vhost : "/" + vhost);
}
