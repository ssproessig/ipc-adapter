#pragma once

#include "Core/api/IConverter.h"



namespace IpcAdapter
{
    namespace Components
    {
        namespace Converters
        {
            /**
             * An IpcAdapter converter that parses the pipelines input as XML
             * and publishes them as meta-data to the pipeline.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class XmlToMetaDataConverter
                : public Core::IConverter
            {
                NONCOPYABLE(XmlToMetaDataConverter);
                PIMPLED_DATA(d);

            public:
                XmlToMetaDataConverter();
                ~XmlToMetaDataConverter() override;

                /// @name IComponent realization
                /// @{
                /**
                 * Configures XmlToMetaDataConverter. Supported parameters
                 * - `filter-on-broken-xml` defaults to `true` - stops processing the frame in the pipeline if non-XML was received
                 * - `forward-raw-data` defaults to `false` - also forward raw data if XML processing was successful
                 *
                 * @return configurable for the XmlToMetaDataConverter
                 */
                Core::IConfigurable* getConfigurable() override;
                /// @}

                /// @name IConverter realization
                /// @{
                /**
                 * Takes the raw data from `anInput` as XML, parses it and forwards it as meta-data.
                 *
                 * @param anInput               frame carrying the raw data to XML parse
                 * @param aStopProcessingFlag   set to `true` if converter is configured to filter non-XML input (default)
                 *
                 * @return raw data from input and XML information as meta-data
                 */
                Core::PipelineFramePtr convert(Core::PipelineFramePtr const& anInput, bool& aStopProcessingFlag) override;
                /// @}
            };
        }
    }
}
