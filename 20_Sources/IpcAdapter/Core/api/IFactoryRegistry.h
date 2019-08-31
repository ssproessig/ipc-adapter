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
            INTERFACE_DESTRUCTOR(IFactoryRegistry)

        public:
            /// a function that creates a shared_ptr of a component
            using FactoryFnc = std::function<std::shared_ptr<IComponent>(void)>;

            /**
             * Queries the factory for component `aComponentId`.
             *
             * @param aComponentId  component id we query a factory for
             *
             * @retval  nullptr		if no factory was found
             * @retval  !=nullptr	if a factory was found for a component identified by `aComponentId`
             */
            virtual FactoryFnc getFactoryFor(QString const& aComponentId) = 0;

        };
    }
}
