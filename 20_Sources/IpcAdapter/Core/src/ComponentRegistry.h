#pragma once

#include "Core/api/IFactoryRegistry.h"



namespace IpcAdapter
{
    namespace Core
    {
        class ComponentRegistry:
            IFactoryRegistry
        {
            NONCOPYABLE(ComponentRegistry);
            PIMPLED_DATA(d);

        public:
            ComponentRegistry();
            virtual ~ComponentRegistry();

            /// @name IFactoryRegistry realization
            /// @{
            FactoryFnc getFactoryFor(QString const& aComponentId) override;
            /// @}

            bool addFactoryFor(QString const& aComponentId, FactoryFnc const& aFactoryFunction);
        };
    }
}
