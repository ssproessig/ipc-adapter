#include "UdpSourceTest.h"

#include "Components/Sources/src/UdpSource.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Components::Sources::UdpSourceTest;
using IpcAdapter::Components::Sources::UdpSource;



UdpSourceTest::UdpSourceTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-SOURCE-001");
}



void UdpSourceTest::test_00_UdpSource_is_registered_in_global_factory_registry()
{
    VERIFY(
        IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor("UdpSource") != nullptr,
        "Ensure the UdpSource is published to global registry"
    );
}
