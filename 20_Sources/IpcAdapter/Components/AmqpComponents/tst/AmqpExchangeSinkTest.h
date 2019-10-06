#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace AmqpComponents
        {
            class AmqpExchangeSinkTest : public QObject
            {
                Q_OBJECT

            public:
                AmqpExchangeSinkTest();

            private slots:
                static void test_00_AmqpExchangeSink_is_registered_in_global_factory_registry();

                static void test_01_AmqpExchangeSink_default_parameters();

                static void test_20_sending_must_fail_if_not_configured();
                static void test_21_sending_with_default_parameters_succeeds();
                static void test_22_sending_with_routing_key_replacement_works();
            };
        }
    }
}
