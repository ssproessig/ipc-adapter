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
            ~SimplePipelineFrame() override;

            /**
             * Override constructor that allows initializing the data buffer directly
             *
             * @param anInitialDataBuffer  the buffer to initialize the pipeline frame with
             */
            explicit SimplePipelineFrame(RawData const& anInitialDataBuffer);

            /**
             * Override constructor that allows initializing from another IPipelineFrame
             *
             * @param anotherPipelineFrame  other pipeline frame to initialize from
             */
            explicit SimplePipelineFrame(std::shared_ptr<IPipelineFrame> const& anotherPipelineFrame);

            /**
             * Updates the frame's raw data to `aRawDataBuffer`.
             *
             * @param aRawDataBuffer    raw data to update the frame with
             */
            void setData(RawData const& aRawDataBuffer);

            /**
             * Updates new meta-data key=value pair in the frame.
             *
             * If an entry for `aKey` does not yet exist, it is added. If it did, it will be overwritten.
             *
             * @param   aKey    meta-data to write
             * @param   aValue  meta-value to write
             */
            void updateMetaData(QString const& aKey, QVariant const& aValue);


            /// @name IPipelineFramer realization
            /// @{
            /**
             * Queries the raw data of the frame.
             *
             * @return raw data stored in the frame
             */
            RawData const& getData() const override;

            /**
             * Queries the frame's meta data.
             *
             * @return meta information of the frame
             */
            MetaDataMap const& getMetaData() const override;
            /// @}
        };
    }
}
