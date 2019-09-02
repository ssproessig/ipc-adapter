#pragma once

#include "Core/api/IRuntimeConfiguration.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(ISource);

        /**
         * Concrete realization of the Runtime Configuration.
         *
         * Shared between @see Runtime and @see ConfigurationHandler.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class RuntimeConfiguration
            : public IRuntimeConfiguration
        {
            NONCOPYABLE(RuntimeConfiguration);
            PIMPLED_DATA(d);

        public:
            RuntimeConfiguration();
            virtual ~RuntimeConfiguration() override;

            /// @name IRuntimeConfiguration realization
            /// @{
            /**
             * Queries all dynamically created components as mapping of "component id" to "instance".
             *
             * @return map of all dynamically created components
             */
            ComponentMap const& getComponents() const override;
            /// @}

            /**
             * Queries the RuntimeConfiguration it a component identified by `aComponentId` already exists.
             *
             * @param   aComponentId    component id to look up
             *
             * @retval true     if `aComponentId` already exists
             * @retval false    if `aComponentId` does not yet exist
             */
            bool containsComponentCalled(QString const& aComponentId);

            /**
             * Adds the `aComponent` instance to the RuntimeConfiguration, identified by `aComponentId`.
             *
             * @param   aComponentId    component id to reference the component under
             * @param   aComponent      component to store in the configuration
             */
            void addComponent(QString const& aComponentId, ComponentPtr const& aComponent);

            /**
             * Queries the runtime configuration for the multiplex of the source identified with `aSourceId`.
             *
             * It no source exists by that name (or the component is no source) a `std::logic_error` is thrown.
             *
             * Otherwise always the same multiplex for that source will be returned, creating it on the first call.
             *
             * @param aSourceId     source id to get the multiplex for
             *
             * @return  multiplex source for the requested source
             *
             * @throws std::logic_error
             */
            ISource* getSourceMultiplexFor(QString const& aSourceId);


            /**
             * Adds the `aPipeline` instance to the RuntimeConfiguration, identified by `aPipelineId`.
             *
             * @param   aPipelineId    pipeline id to reference the pipeline under
             * @param   aPipeline      pipeline to store in the configuration
             *
             * @throws std::logic_error  if existing `aPipelineId` is reused
             */
            void addPipeline(QString const& aPipelineId, PipelinePtr const& aPipeline);
        };
    }
}
