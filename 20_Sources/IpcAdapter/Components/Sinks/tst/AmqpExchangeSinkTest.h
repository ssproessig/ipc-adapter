#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace Sinks
        {
            class AmqpExchangeSinkTest : public QObject
            {
                Q_OBJECT

            public:
                AmqpExchangeSinkTest();

            private slots:
                static void test_00_AmqpExchangeSink_is_registered_in_global_factory_registry();

                static void test_01_AmqpExchangeSink_default_parameters();

                static void test_02_AmqpExchangeSink_configuring_unsupported_host_must_fail();
                static void test_03_AmqpExchangeSink_configuring_unsupported_port_must_fail();
                static void test_04_AmqpExchangeSink_configuring_unsupported_vhost_must_fail();
                static void test_05_AmqpExchangeSink_configuring_unsupported_exchange_type_must_fail();
                static void test_06_AmqpExchangeSink_configuring_unsupported_routing_key_must_fail();
                static void test_07_AmqpExchangeSink_configuring_unsupported_parameter_must_fail();

            };
        }
    }
}
