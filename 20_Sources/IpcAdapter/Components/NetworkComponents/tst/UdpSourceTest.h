#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace NetworkComponents
        {
            class UdpSourceTest : public QObject
            {
                Q_OBJECT

            public:
                UdpSourceTest();

            private slots:
                void test_00_UdpSource_is_registered_in_global_factory_registry();

                void test_01_configuration_fails_for_unknown_parameter();
                void test_02_configuration_succeeds_for_known_parameters_w_correct_value();
                void test_04_configuration_fails_for_invalid_port();
                void test_05_configuration_fails_for_invalid_host();

                void test_10_forwarding_to_pipeline_succeeds_for_default_configuration();
                void test_11_forwarding_to_pipeline_succeeds_for_configured_parameters();
            };
        }
    }
}
