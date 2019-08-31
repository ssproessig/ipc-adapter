#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include <functional>

FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IComponent);

        class IFactoryRegistry
        {
        public:
            INTERFACE_DESTRUCTOR(IFactoryRegistry)

        public:
            using FactoryFnc = std::function<std::shared_ptr<IComponent>(void)>;

            virtual FactoryFnc getFactoryFor(QString const& aComponentId) = 0;

        };
    }
}
