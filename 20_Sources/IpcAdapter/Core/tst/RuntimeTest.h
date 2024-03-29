#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Core
    {
        class RuntimeTest : public QObject
        {
            Q_OBJECT

        public:
            RuntimeTest();

        private slots:
            void init();
            void test_01_Runtime_initialization_fails_if_not_existing_file_is_passed();
            void test_02_Runtime_initialization_fails_for_broken_file();
            void test_03_Runtime_initialization_fails_for_wrong_xml_used();
            void test_04_Runtime_initialization_fails_for_wrong_root_element();
            void test_05_Runtime_initialization_fails_for_root_element_with_unsupported_version();

            void test_06_Runtime_initialization_fails_if_unknown_component_shall_be_created();
            void test_07_Runtime_initialization_fails_if_duplicate_component_shall_be_created();
            void test_08_Runtime_initialization_fails_if_component_fails_to_configure();
            void test_09_Runtime_initialization_fails_if_component_rejects_parameter();

            void test_10_Runtime_initialization_fails_if_unknown_param_list_is_referenced();

            void test_20_Runtime_initialization_fails_if_duplicate_pipeline_id_is_used();
            void test_21_Runtime_initialization_fails_if_pipeline_uses_non_sink_as_sink();
            void test_22_Runtime_initialization_fails_if_pipeline_uses_non_converter_as_converter();

            void test_98_Runtime_initialization_succeeds();
            void test_99_Runtime_initialization_works_for_component_that_is_not_configurable();
        };
    }
}

