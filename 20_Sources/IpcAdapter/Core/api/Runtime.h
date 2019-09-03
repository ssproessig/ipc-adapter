#pragma once

#include "Core/api/IRuntime.h"

FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * Static helper class used to instantiate the concrete Runtime with from the configuration file passed.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class Runtime
        {
            STATIC_CLASS(Runtime);

        public:

            /**
             * Creates a new runtime and configures it with configuration file passed in `aConfigurationFile`.
             *
             * May throw `std::runtime_error` if parsing the configuration fails.
             *
             * @throw std::runtime_error  in case parsing fails
             *
             * @return  Runtime created after parsing `aConfigurationFile`
             */
            static std::unique_ptr<IRuntime> createFrom(QString const& aConfigurationFile);

        };

    }
}
