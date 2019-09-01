#pragma once

#include "Core/api/IRuntimeConfiguration.h"



namespace IpcAdapter
{
    namespace Core
    {
        class RuntimeConfiguration
            : public IRuntimeConfiguration
        {
            NONCOPYABLE(RuntimeConfiguration);
            PIMPLED_DATA(d);

        public:
            RuntimeConfiguration();
            virtual ~RuntimeConfiguration() override;

            ComponentMap const& getComponents() const override;

            bool containsComponentCalled(QString const& aComponentId);
            void addComponent(QString const& aComponentId, ComponentPtr const& aComponent);
        };
    }
}
