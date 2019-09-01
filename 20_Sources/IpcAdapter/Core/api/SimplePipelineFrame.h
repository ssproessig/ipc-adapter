#pragma once

#include "Core/api/IPipelineFrame.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * Concrete simple realization of a pipeline frame.
         *
         * Raw data can be set and queried.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class SimplePipelineFrame
            : public IPipelineFrame
        {
            NONCOPYABLE(SimplePipelineFrame);
            PIMPLED_DATA(d);

        public:
            SimplePipelineFrame();
            virtual ~SimplePipelineFrame() override;

            /**
             * Updates the frame's raw data to `aRawDataBuffer`.
             *
             * @param aRawDataBuffer    raw data to update the frame with
             */
            void setData(QByteArray const& aRawDataBuffer);

            /// @name IPipelineFramer realization
            /// @{
            /**
             * Queries the raw data of the frame.
             *
             * @return raw data stored in the frame
             */
            QByteArray const& getData() const override;
            /// @}
        };
    }
}
