#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace Sources
        {
            class UdpSourceTest : public QObject
            {
                Q_OBJECT

            public:
                UdpSourceTest();

            private slots:
                void test_00_UdpSource_is_registered_in_global_factory_registry();

            };
        }
    }
}
