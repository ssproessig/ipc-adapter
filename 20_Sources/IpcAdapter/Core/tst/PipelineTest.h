#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Core
    {
        class PipelineTest : public QObject
        {
            Q_OBJECT

        public:
            PipelineTest();

        private slots:

            void test_01_source_to_multiple_sinks();
            void test_02_dont_create_multiplex_for_unknown_source();
            void test_03_source_multiplex_uses_all_pipelines();

        };
    }
}

