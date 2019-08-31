#include "ComponentRegistryTest.h"

#include "Core/src/ComponentRegistry.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Core::ComponentRegistryTest;
using IpcAdapter::Core::ComponentRegistry;



ComponentRegistryTest::ComponentRegistryTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("D-IPCA-005");
}



void ComponentRegistryTest::test_01_querying_unknown_component_factory_returns_no_functor()
{
    ComponentRegistry uut;

    COMPARE(uut.getFactoryFor("unknown"), nullptr, "Unknown component SHALL return no factory");
}
