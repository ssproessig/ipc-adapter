#pragma once

#include "Shared/api/ConvenienceMacros.h"

FORWARD_DECLARE_TEMPLATED_2(QMap);
FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IComponent);
        FORWARD_DECLARE(IConfigurator);
        FORWARD_DECLARE(IPipeline);

        /// smart pointer carrying a configurator
        using ConfiguratorPtr = std::shared_ptr<IConfigurator>;

        /// mapping of configuratorId to its instance
        using ConfiguratorMap = QMap<QString, ConfiguratorPtr>;

        /// smart pointer carrying a shared component
        using ComponentPtr = std::shared_ptr<IComponent>;

        /// mapping of componentId to its instance
        using ComponentMap = QMap<QString, ComponentPtr>;

        /// smart pointer carrying a shared pipeline
        using PipelinePtr = std::shared_ptr<IPipeline>;

        /// mapping of pipelineId to its instance
        using PipelineMap = QMap<QString, PipelinePtr>;

        /**
         * An interface used to read the dynamic runtime configuration.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IRuntimeConfiguration
        {
        public:
            INTERFACE_DESTRUCTOR(IRuntimeConfiguration);

            /**
             * Queries all dynamically created components as mapping of "component id" to "instance".
             *
             * @return map of all dynamically created components
             */
            virtual ComponentMap const& getComponents() const = 0;

            /**
             * Queries the runtime configuration for a component by its `aComponentId`
             *
             * @param aComponentId  the id of the component to query
             *
             * @retval  nullptr   empty smart pointer if no component with that id exists
             * @retval  !=nullptr shared pointer to the component
             */
            virtual ComponentPtr getComponent(QString const& aComponentId) const = 0;

            /**
             * Queries all dynamically created pipelines as mapping of "pipeline id" to "instance".
             *
             * @return map of all dynamically created pipelines
             */
            virtual PipelineMap const& getPipelines() const = 0;

            /**
             * Queries the runtime configuration for a parameter-list by its `aParamListId`
             *
             * @param aParamListId  the id of the parameter-list to query
             *
             * @retval  nullptr   empty smart pointer if no parameter-list with that id exists
             * @retval  !=nullptr shared pointer to the parameter-list
             */
            virtual ConfiguratorPtr getParamList(QString const& aParamListId) const = 0;
        };
    }
}
