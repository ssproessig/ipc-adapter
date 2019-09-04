#include "AmqpExchangeSinkTest.h"

#include "Core/api/GlobalComponentRegistry.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Components::Sinks::AmqpExchangeSinkTest;



AmqpExchangeSinkTest::AmqpExchangeSinkTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-SINK-003");
}



void AmqpExchangeSinkTest::test_00_AmqpExchangeSink_is_registered_in_global_factory_registry()
{
    VERIFY(
        IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor("AmqpExchangeSink") != nullptr,
        "Ensure the AmqpExchangeSink is published to global registry"
    );
}
