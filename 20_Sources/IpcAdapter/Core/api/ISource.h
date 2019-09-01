#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"



namespace IpcAdapter
{
    namespace Core
    {
        class ISource
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(ISource);
        };
    }
}
