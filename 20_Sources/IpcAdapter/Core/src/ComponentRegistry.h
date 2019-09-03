#pragma once

#include "Core/api/IFactoryRegistry.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * A registry that resolves component factories for the components unique id.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class ComponentRegistry
            : public IFactoryRegistry
        {
            NONCOPYABLE(ComponentRegistry);
            PIMPLED_DATA(d);

        public:
            ComponentRegistry();
            ~ComponentRegistry() override;

            /// @name IFactoryRegistry realization
            /// @{
            /**
             * Queries the factory for component `aComponentId`.
             *
             * @param aComponentId  component id we query a factory for
             *
             * @retval  nullptr		if no factory was found
             * @retval  !=nullptr	if a factory was found for a component identified by `aComponentId`
             */
            FactoryFnc getFactoryFor(QString const& aComponentId) override;

            /**
             * Registers a factory implementation of `aFactoryFunction` in the registry for a component identified by
             * `aComponentId`.
             *
             * @param aComponentId		component id we add the factory for
             * @param aFactoryFunction	factory function to add
             *
             * @retval  true		if the factory was added for `aComponentId`
             * @retval  false		if inserting the factory failed (e.g. duplicate `aComponentId`)
             */
            bool registerFactoryFor(QString const& aComponentId, FactoryFnc const& aFactoryFunction) override;
            /// @}
        };
    }
}
