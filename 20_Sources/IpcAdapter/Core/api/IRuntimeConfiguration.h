#pragma once

#include "Shared/api/ConvenienceMacros.h"

FORWARD_DECLARE_TEMPLATED_2(QMap);
FORWARD_DECLARE(QString);



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IComponent);

        using ComponentPtr = std::shared_ptr<IComponent>;
        using ComponentMap = QMap<QString, ComponentPtr>;


        class IRuntimeConfiguration
        {
        public:
            INTERFACE_DESTRUCTOR(IRuntimeConfiguration);

            virtual ComponentMap const& getComponents() const = 0;
        };
    }
}
