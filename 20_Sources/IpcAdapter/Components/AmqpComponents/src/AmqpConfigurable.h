#pragma once

#include "Core/api/IConfigurable.h"

#include <functional>



namespace IpcAdapter
{
    namespace Components
    {
        namespace AmqpComponents
        {
            FORWARD_DECLARE_STRUCT(AmqpConfiguration);

            class AmqpConfigurable
                : public Core::IConfigurable
            {
                NONCOPYABLE(AmqpConfigurable);
                NONMOVABLE(AmqpConfigurable);
                PIMPLED_DATA(d);

            public:
                using ConfigurationFinishedCallback = std::function<void()>;

                AmqpConfigurable(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback);
                ~AmqpConfigurable() override;

                /// @name IConfigurable implementation
                /// @{
                void onConfigureBegin() override;

                bool doConfigure(QString const& aKey, QString const& aValue) override;

                bool onConfigureEnd() override;
                /// @}
            };
        }
    }
}
