#pragma once

#include "Shared/api/ConvenienceMacros.h"

FORWARD_DECLARE(QByteArray);



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An interface used to represent a frame on a pipeline.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IPipelineFrame
        {
        public:
            INTERFACE_DESTRUCTOR(IPipelineFrame);

            /// alias for the raw data-type of a frame
            using RawData = QByteArray;

            /**
             * Queries the raw data of the frame.
             *
             * @return raw data of the frame
             */
            virtual RawData const& getData() const = 0;
        };
    }
}
