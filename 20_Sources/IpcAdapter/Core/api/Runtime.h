#pragma once

#include "Core/api/IRuntime.h"

FORWARD_DECLARE(QString);



namespace Core
{

    class Runtime
    {
        STATIC_CLASS(Runtime);
    public:
        static std::unique_ptr<IRuntime> createFrom(QString const& aConfigurationFile);

    };

}
