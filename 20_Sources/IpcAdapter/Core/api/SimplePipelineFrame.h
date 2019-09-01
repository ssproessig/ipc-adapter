#pragma once

#include "Core/api/IPipelineFrame.h"



namespace IpcAdapter
{
    namespace Core
    {
        class SimplePipelineFrame
            : public IPipelineFrame
        {
            NONCOPYABLE(SimplePipelineFrame);
            PIMPLED_DATA(d);

        public:
            SimplePipelineFrame();
            virtual ~SimplePipelineFrame() override;

            void setData(QByteArray const& aRawDataBuffer);

            /// @name IPipelineFramer realization
            /// @{
            QByteArray const& getData() const override;
            /// @}
        };
    }
}
