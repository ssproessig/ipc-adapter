#pragma once

#include "Shared/api/ConvenienceMacros.h"

FORWARD_DECLARE(QByteArray);
FORWARD_DECLARE_TEMPLATED_2(QMap);
FORWARD_DECLARE(QString);
FORWARD_DECLARE(QVariant);



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An interface used to represent a frame on a pipeline.
         *
         * A frame carries
         * - raw data
         * - meta-info as key-value pairs
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

            /// alias for the meta data-type of a frame
            using MetaDataMap = QMap<QString, QVariant>;

            /**
             * Queries the raw data of the frame.
             *
             * @return raw data of the frame
             */
            virtual RawData const& getData() const = 0;

            /**
             * Queries the frame's meta data.
             *
             * @return meta information of the frame
             */
            virtual MetaDataMap const& getMetaData() const = 0;
        };
    }
}
