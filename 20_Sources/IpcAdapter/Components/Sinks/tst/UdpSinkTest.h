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
                void test_01_configuration_fails_for_unknown_parameter();
                void test_02_configuration_succeeds_for_known_parameters_w_correct_value();
                void test_03_configuration_fails_if_mandatory_parameter_is_missing();
                void test_04_configuration_fails_for_invalid_port();
                void test_05_configuration_fails_for_invalid_host();
            };
        }
    }
}
