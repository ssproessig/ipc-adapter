#pragma once

#include "Shared/api/ConvenienceMacros.h"

FORWARD_DECLARE_TEMPLATED_2(QMap);
FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IComponent);

        /// smart pointer carrying a shared component
        using ComponentPtr = std::shared_ptr<IComponent>;

        /// mapping of componentId to its instance
        using ComponentMap = QMap<QString, ComponentPtr>;

        /**
         * An interface used to read the dynamic runtime configuration.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IRuntimeConfiguration
        {
        public:
            INTERFACE_DESTRUCTOR(IRuntimeConfiguration);

            /**
             * Queries all dynamically created components as mapping of "component id" to "instance".
             *
             * @return map of all dynamically created components
             */
            virtual ComponentMap const& getComponents() const = 0;
        };
    }
}
