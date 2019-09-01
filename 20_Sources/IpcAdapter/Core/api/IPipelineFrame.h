#pragma once

#include "Shared/api/ConvenienceMacros.h"

FORWARD_DECLARE(QByteArray);



namespace IpcAdapter
{
    namespace Core
    {
        class IPipelineFrame
        {
        public:
            INTERFACE_DESTRUCTOR(IPipelineFrame);

            virtual QByteArray const& getData() const = 0;
        };
    }
}
