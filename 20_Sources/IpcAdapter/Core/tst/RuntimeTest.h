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

        };
    }
}

