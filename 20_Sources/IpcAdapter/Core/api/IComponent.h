#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IConfigurable);

        class IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(IComponent)

            virtual IConfigurable& getConfigurable() = 0;

        };
    }
}
