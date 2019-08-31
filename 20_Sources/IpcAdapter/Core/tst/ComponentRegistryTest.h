#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Core
    {
        class ComponentRegistryTest : public QObject
        {
            Q_OBJECT

        public:
            ComponentRegistryTest();

        private slots:

            void test_01_querying_unknown_component_factory_returns_no_functor();

        };
    }
}

