# AMQP Components

## R-IPCA-AMQPCFG-001: Configuring AMQP parameters
The module `AmqpComponents` SHALL provide an `IConfigurable` that supports shared configuring AMQP parameters. It SHALL be called `AmqpConfigurable`.

The `AmqpConfigurable` SHALL support configuring

- `amqp.host`  the host as `QHostAddress` the AMQP server runs at
- `amqp.port`  the port as `quint16` the AMQP server runs at

- `auth.user`  the username as `QString` used to authenticate at the AMQP server
- `auth.pwd`  the password as `QString` used to authenticate at the AMQP server
- `auth.vhost`  the _virtual host_ as `QString` used to authenticate at the AMQP server

- `exchange.name` the name of the exchange as `QString` to use
- `exchange.type` the type of the exchange as `QString` to use; the supported values SHALL be
    - `topic` to create a _topic_ exchange
    - `direct` to create a _direct_ exchange
    - `fanout` to create a _direct_ exchange
    - _header_ exchanges SHALL not be supported

- `exchange.routing-key` the routing-key to use when publishing to a `topic` exchange

## R-IPCA-AMQPCFG-002: AMQP default server values
The `AmqpConfigurable` SHALL use the following default server configuration values:

- `amqp.host` shall default to `127.0.0.1`
- `amqp.port` shall default to `5672`

## R-IPCA-AMQPCFG-003: AMQP default authentication values
The `AmqpConfigurable` SHALL use the [default authentication values of RabbitMQ](https://www.rabbitmq.com/access-control.html#default-state):

- `auth.user` shall default to `guest`
- `auth.pwd` shall default to `guest`
- `auth.vhost` shall default to `/`

## R-IPCA-AMQPCFG-004: AMQP default exchange values
The `AmqpConfigurable` SHALL use the [default exchange value of RabbitMQ](https://www.rabbitmq.com/tutorials/amqp-concepts.html#exchange-default):

- `exchange.name` shall default to ` ` (empty string)
- `exchange.type` shall default to `direct`


## R-IPCA-AMQPSNK-001: Provide AmqpExchangeSink
The system SHALL provide a sink called `AmqpExchangeSink` that provides means to send pipeline data to an AMQP exchange.

## R-IPCA-AMQPSNK-002: AmqpExchangeSink routing-key meta-data replacement
The `exchange.routing-key` SHALL support meta-data replacement. If a meta-data key occurs as escaped `${meta-data.key}` placeholer, the meta-data value SHALL be used.

Example: 
- meta-data key `key` = `value123`
- if `exchange.routing-key` is defined as `abc.${key}.xyz` it SHALL be replace as `abc.value123.xyz`
