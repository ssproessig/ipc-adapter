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
            void test_01_Runtime_initialization_fails_if_not_existing_file_is_passed();
            void test_02_Runtime_initialization_fails_for_broken_file();

        };
    }
}

