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
             * Override constructor that allows initializing the data buffer directly
             *
             * @param anInitialDataBuffer  the buffer to initialize the pipeline frame with
             */
            explicit SimplePipelineFrame(QByteArray const& anInitialDataBuffer);

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
