#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace AmqpComponents
        {
            class AmqpConfigurableTest : public QObject
            {
                Q_OBJECT

            public:
                AmqpConfigurableTest();

            private slots:
                static void test_01_configuring_unsupported_host_must_fail();
                static void test_02_configuring_unsupported_port_must_fail();
                static void test_03_configuring_unsupported_vhost_must_fail();
                static void test_04_configuring_unsupported_exchange_type_must_fail();
                static void test_05_configuring_unsupported_routing_key_must_fail();
                static void test_06_configuring_unsupported_parameter_must_fail();
            };
        }
    }
}
