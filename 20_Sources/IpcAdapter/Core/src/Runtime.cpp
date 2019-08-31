#include "Core/api/Runtime.h"
#include "Core/api/Logger.h"


using IpcAdapter::Core::Runtime;
using IpcAdapter::Core::IRuntime;



namespace
{
    class RuntimeImpl: public IRuntime
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



std::unique_ptr<IRuntime> Runtime::createFrom(QString const& aConfigurationFile)
{
    LOG_DEBUG(nullptr) << "Creating Runtime from " << aConfigurationFile;

    return std::make_unique<RuntimeImpl>();
}
