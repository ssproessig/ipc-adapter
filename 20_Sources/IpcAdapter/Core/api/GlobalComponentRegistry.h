#pragma once

#include "Shared/api/ConvenienceMacros.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IFactoryRegistry);

        /**
         * The global component registry following the Singleton pattern.
         *
         * Simplifies registering components from their implementation.
         *
         * NOTE: does only work as long as we have static libraries! With dynamically libraries this must be changed
         *       to a registration API per library.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class GlobalComponentRegistry
        {
            STATIC_CLASS(GlobalComponentRegistry);

        public:
            /**
             * Returns the one-and-only factory registry used in the Core.
             *
             * @return one-and-only factory registry in the Core.
             */
            static IFactoryRegistry& get();

        };
    }
}
