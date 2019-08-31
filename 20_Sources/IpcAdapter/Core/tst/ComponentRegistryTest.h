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
            void test_02_adding_a_factory_succeeds();
            void test_03_adding_a_component_twice_fails();
            void test_04_querying_known_component_factory_returns_functor_that_creates_a_component();

            void test_99_global_component_registry_is_singleton();

        };
    }
}

