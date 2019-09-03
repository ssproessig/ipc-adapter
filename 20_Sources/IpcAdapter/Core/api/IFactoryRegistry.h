#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include <functional>

FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IComponent);

        /**
         * An abstraction of a registry that is used to lookup component factories by component name.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IFactoryRegistry
        {
        public:
            INTERFACE_DESTRUCTOR(IFactoryRegistry);

            /// a function that creates a shared_ptr of a component
            using FactoryFnc = std::function<std::shared_ptr<IComponent>()>;

            /**
             * Queries the factory for component `aComponentId`.
             *
             * @param aComponentId  component id we query a factory for
             *
             * @retval  nullptr		if no factory was found
             * @retval  !=nullptr	if a factory was found for a component identified by `aComponentId`
             */
            virtual FactoryFnc getFactoryFor(QString const& aComponentId) = 0;

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
            virtual bool registerFactoryFor(QString const& aComponentId, FactoryFnc const& aFactoryFunction) = 0;
        };
    }
}
