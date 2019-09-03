#pragma once

#include "Core/api/IConverter.h"



namespace IpcAdapter
{
    namespace Components
    {
        namespace Converters
        {
            /**
             * An IpcAdapter converter that reverses the bytes forwarded to it.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class ReverseBytesConverter
                : public Core::IConverter
            {
                NONCOPYABLE(ReverseBytesConverter);

            public:
                ReverseBytesConverter() = default;
                virtual ~ReverseBytesConverter() override = default;

                /// @name IComponent realization
                /// @{
                /**
                 * ReverseBytesConverter is not configurable
                 *
                 * @return nullptr  there is no configurable to call
                 */
                Core::IConfigurable* getConfigurable() override;
                /// @}

                /// @name IConverter realization
                /// @{
                /**
                 * Takes the raw data from `anInput` and reverses their order.
                 *
                 * @param anInput  frame carrying the raw data to reverse
                 *
                 * @return reversed raw data from input
                 */
                Core::PipelineFramePtr convert(Core::PipelineFramePtr const& anInput) override;
                /// @}
            };
        }
    }
}
