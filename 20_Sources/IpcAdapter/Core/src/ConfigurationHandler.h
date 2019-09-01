#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include <QXmlDefaultHandler>



namespace IpcAdapter
{
    namespace Core
    {
        class ConfigurationHandler:
            public QXmlDefaultHandler
        {
            NONCOPYABLE(ConfigurationHandler);

        public:
            ConfigurationHandler() = default;
            virtual ~ConfigurationHandler() = default;

        };

    }
}
