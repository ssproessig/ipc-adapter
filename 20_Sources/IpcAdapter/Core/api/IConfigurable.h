#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"

FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        class IConfigurable
        {
        public:
            INTERFACE_DESTRUCTOR(IConfigurable);

            virtual void onConfigureBegin() = 0;

            virtual bool doConfigure(QString const& aKey, QString const& aValue) = 0;

            virtual bool onConfigureEnd() = 0;

        };
    }
}
