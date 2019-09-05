#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace Converters
        {
            class XmlToMetaDataConverterTest : public QObject
            {
                Q_OBJECT

            public:
                XmlToMetaDataConverterTest();

            private slots:
                static void test_00_XmlToMetaDataConverter_is_published();
                static void test_01_configuration_fails_for_unknown_parameter();
                static void test_02_configuration_fails_for_non_boolean_filter();
                static void test_03_configuration_fails_for_non_boolean_forward();

                static void test_10_valid_processing();
                static void test_11_filter_on_broken_xml();
                static void test_12_overwrite_default_parameters_must_work();
            };
        }
    }
}
