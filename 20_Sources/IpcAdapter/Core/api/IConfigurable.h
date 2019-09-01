#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"

FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An interface realized by components that are configurable at runtime.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IConfigurable
        {
        public:
            INTERFACE_DESTRUCTOR(IConfigurable);

            /**
             * Starts runtime configuration. Used to reinitialize the component's configuration (like setting defaults).
             */
            virtual void onConfigureBegin() = 0;

            /**
             * Configures the target
             *
             * @param aKey   identification of the parameter to set
             * @param aValue value to set the parameter identified by `aKey` to
             *
             * @retval true  if the parameter was successfully set and the value was converted
             * @retval false if the parameter failed to be set
             */
            virtual bool doConfigure(QString const& aKey, QString const& aValue) = 0;

            /**
             * Finishes runtime configuration.  Used to initialize the component from all the parameters provided.
             *
             * @retval true  if the component successfully finished configuring from the parameters provided
             * @retval false if the component failed to configured itself
             */
            virtual bool onConfigureEnd() = 0;

        };
    }
}
