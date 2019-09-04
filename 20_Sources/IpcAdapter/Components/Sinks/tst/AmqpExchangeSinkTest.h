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

            };
        }
    }
}
