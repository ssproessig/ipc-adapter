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
        };
    }
}
