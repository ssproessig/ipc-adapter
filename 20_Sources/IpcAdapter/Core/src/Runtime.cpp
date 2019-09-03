#include "Core/api/Runtime.h"
#include "Core/api/Logger.h"

#include "Core/src/ConfigurationHandler.h"
#include "Core/src/RuntimeConfiguration.h"

#include <QFile>
#include <QXmlSimpleReader>



using IpcAdapter::Core::Runtime;
using IpcAdapter::Core::IRuntime;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, exceptionFileNotFound, QStringLiteral("Given configuration '%1' not found!"))
    }

    struct RuntimeImpl: IRuntime
    {
        void configure(QString const& aConfigurationFile)
        {
            LOG_DEBUG(this) << "Runtime configuring from" << aConfigurationFile;

            IpcAdapter::Core::ConfigurationHandler handler(configuration);

            QXmlSimpleReader reader;
            reader.setContentHandler(&handler);
            reader.setErrorHandler(&handler);

            QFile xmlFile(aConfigurationFile);

            if (!xmlFile.exists())
            {
                THROW(Constants::exceptionFileNotFound().arg(aConfigurationFile));
            }

            QXmlInputSource source(&xmlFile);

            if (reader.parse(source))
            {
                // FIXME: create pipeline
            }
            else
            {
                // TODO: check if we need to embed the parser error in more context
                THROW(handler.errorString());
            }
        }

        IpcAdapter::Core::IRuntimeConfiguration const& getRuntimeConfiguration() const override
        {
            return configuration;
        }

        void serveForever() override
        {
            LOG_DEBUG(this) << "Runtime serving forever...";
        }

        IpcAdapter::Core::RuntimeConfiguration configuration;
    };
}



std::unique_ptr<IRuntime> Runtime::createFrom(QString const& aConfigurationFile)
{
    LOG_DEBUG(nullptr) << "Creating Runtime from " << aConfigurationFile;

    auto runtime = std::make_unique<RuntimeImpl>();
    runtime->configure(aConfigurationFile);
    return std::move(runtime);
}
