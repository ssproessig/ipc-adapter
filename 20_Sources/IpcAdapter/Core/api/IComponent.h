#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IConfigurable);

        /**
         * An common interface representing a any dynamically created component in the Runtime.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(IComponent);

            /**
             * Queries the component for its configuration means.
             *
             * May return `nullptr` if the component is not configurable.
             *
             * @retval !=nullptr    the component's configuration implementation
             * @retval nullptr      the component's is not configurable
             */
            virtual IConfigurable* getConfigurable() = 0;
        };
    }
}
