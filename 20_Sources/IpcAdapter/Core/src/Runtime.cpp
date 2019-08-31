#include "Core/api/Runtime.h"
#include "Core/api/Logger.h"


using Core::Runtime;



namespace
{
    class RuntimeImpl: public Core::IRuntime
    {
        void configure() override
        {
            LOG_DEBUG(this) << "Runtime configuring...";
        }

        void serveForever() override
        {
            LOG_DEBUG(this) << "Runtime serving forever...";
        }
    };
}



std::unique_ptr<Core::IRuntime> Runtime::createFrom(QString const& aConfigurationFile)
{
    LOG_DEBUG(nullptr) << "Creating Runtime from " << aConfigurationFile;

    return std::make_unique<RuntimeImpl>();
}
