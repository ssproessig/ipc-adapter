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

            private slots:
                void testConfigurable();
            };
        }
    }
}
