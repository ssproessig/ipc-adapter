#include "AmqpConfigurable.h"
#include "AmqpConfiguration.h"

#include "Core/api/Logger.h"

#include <QList>
#include <QMap>



using IpcAdapter::Components::AmqpComponents::AmqpConfigurable;
using IpcAdapter::Components::AmqpComponents::AmqpConfiguration;



namespace
{
    namespace Constants
    {
        DECLARE_CONST_VARIADIC(QList<QString>, supportedExchangeTypes, {"topic", "direct", "fanout"});
    }
}



struct AmqpConfigurable::Data
{
    using SupportedParameters = QMap<QString, std::function<bool(AmqpConfiguration&, QString const&)>>;

    AmqpConfiguration& configuration;
    ConfigurationFinishedCallback callback;
    SupportedParameters parameterHandler;


#define PARAM_START(NAME) parameterHandler.insert(NAME, [](AmqpConfiguration & c, auto const & value)
#define PARAM_END )
#define PARAM_SETTER_ALWAYS(NAME, FIELD) PARAM_START(NAME) { c.FIELD = value; return true; } PARAM_END
#define PARAM_SETTER_VALIDATE(NAME, FIELD, VALIDATOR) PARAM_START(NAME) { c.FIELD = value; return VALIDATOR; } PARAM_END


    Data(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback)
        : configuration(aConfiguration)
        , callback(aCallback)
    {
        REALIZE_REQUIREMENT("R-IPCA-AMQPSNK-002");

        PARAM_SETTER_VALIDATE("auth.vhost", vhost, !value.isEmpty());
        PARAM_SETTER_ALWAYS("auth.user", user);
        PARAM_SETTER_ALWAYS("auth.pwd", pwd);
        PARAM_SETTER_ALWAYS("exchange.name", exchangeName);
        PARAM_SETTER_VALIDATE("exchange.routing-key", routingKey, !value.isEmpty());
        PARAM_SETTER_VALIDATE("exchange.type", exchangeType, Constants::supportedExchangeTypes().contains(value));
        PARAM_START("amqp.host")
        {
            return c.host.setAddress(value);
        }
        PARAM_END;
        PARAM_START("amqp.port")
        {
            auto conversionOk = false;
            auto const& port = value.toInt(&conversionOk);
            conversionOk = conversionOk &&
                           port >= std::numeric_limits<quint16>::min() && port <= std::numeric_limits<quint16>::max();

            c.port = static_cast<quint16>(port);
            return conversionOk;
        }
        PARAM_END;
    }


#undef PARAM_END
#undef PARAM_SETTER_ALWAYS
#undef PARAM_SETTER_VALIDATE
#undef PARAM_START

};



void AmqpConfigurable::onConfigureBegin()
{
    // FIXME: disconnectFromHost
    d->configuration.initDefaults();
}


bool AmqpConfigurable::doConfigure(QString const& aKey, QString const& aValue)
{
    LOG_DEBUG(this) << "configuring " << aKey << "to" << aValue;

    auto const& it = d->parameterHandler.constFind(aKey);

    if (it != d->parameterHandler.constEnd())
    {
        auto const configured = it.value()(d->configuration, aValue);
        LOG_DEBUG(this) << "result" << configured;

        if (!configured)
        {
            d->configuration.isOk = false;
        }

        return configured;
    }

    LOG_ERROR(this) << "unsupported parameter: " << aKey;
    return false;
}


bool AmqpConfigurable::onConfigureEnd()
{
    if (d->configuration.isOk)
    {
        d->callback();
    }

    return d->configuration.isOk;
}



AmqpConfigurable::AmqpConfigurable(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback)
    : d(std::make_unique<Data>(aConfiguration, aCallback))
{}

AmqpConfigurable::~AmqpConfigurable() = default;
