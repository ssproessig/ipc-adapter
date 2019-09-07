#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"

FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An interface realized by components that configure other components.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IConfigurator
        {
        public:
            INTERFACE_DESTRUCTOR(IConfigurator);

            /**
             * Configures the passed `aConfigurable`.
             *
             * @retval true  if the configurator successfully configured `aConfigurable`
             * @retval false if any of the parameters was rejected
             */
            virtual bool doConfigure(IConfigurable& aConfigurable) = 0;
        };
    }
}
