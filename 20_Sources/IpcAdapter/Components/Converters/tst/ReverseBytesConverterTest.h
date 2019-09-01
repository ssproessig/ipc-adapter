#pragma once

#include <QObject>



namespace IpcAdapter
{
    namespace Components
    {
        namespace Converters
        {
            class ReverseBytesConverterTest : public QObject
            {
                Q_OBJECT

            public:
                ReverseBytesConverterTest();

            private slots:
                void test_00_ReverseBytesConverter_is_published();
                void test_01_ReverseBytesConverter_is_not_configurable();
                void test_02_ReverseBytesConverter_conversion();
            };
        }
    }
}
