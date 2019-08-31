#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace Sinks
        {
            class UdpSinkTest : public QObject
            {
                Q_OBJECT

            public:
                UdpSinkTest();

            private slots:
                void test_00_UdpSink_is_registered_in_global_factory_registry();

                void test_01_configuration_fails_for_unknown_parameter();
                void test_02_configuration_succeeds_for_known_parameters_w_correct_value();
                void test_03_configuration_fails_if_mandatory_parameter_is_missing();
                void test_04_configuration_fails_for_invalid_port();
                void test_05_configuration_fails_for_invalid_host();

                void test_10_forwarding_pipeline_frame_succeeds_for_configured_sink();
            };
        }
    }
}
