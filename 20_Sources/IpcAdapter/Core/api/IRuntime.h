#pragma once

#include "Shared/api/ConvenienceMacros.h"


namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IRuntimeConfiguration);

        /**
         * An interface representing the dynamically configured Runtime.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IRuntime
        {
        public:
            INTERFACE_DESTRUCTOR(IRuntime);

            /**
             * Queries the runtime for its dynamically configured configuration.
             *
             * @return  dynamically configured Runtime configuration
             */
            virtual IRuntimeConfiguration const& getRuntimeConfiguration() const = 0;

            /**
             * Switches the Runtime from configuration-time to run-time.
             *
             * No more configuration or exceptions SHALL happen from now.
             */
            virtual void serveForever() = 0;
        };
    }
}
