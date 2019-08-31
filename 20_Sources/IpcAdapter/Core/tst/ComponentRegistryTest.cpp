#include "ComponentRegistryTest.h"

#include "Core/src/ComponentRegistry.h"
#include "Shared/tst/QTestConvenienceMacros.h"

#include "Core/api/IComponent.h"

#include <QString>



using IpcAdapter::Core::ComponentRegistryTest;
using IpcAdapter::Core::ComponentRegistry;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, componentId, ("SampleComponent"));
    }

    class SampleComponent: public IpcAdapter::Core::IComponent
    {
    public:
        IpcAdapter::Core::IConfigurable& getConfigurable() override
        {
            throw std::logic_error("...shall not be called from test anyway");
        }
    };

    bool addSampleComponentFactoryTo(ComponentRegistry& aRegistry)
    {
        return aRegistry.addFactoryFor(Constants::componentId(), []()->std::shared_ptr<IpcAdapter::Core::IComponent>
        {
            return std::make_shared<SampleComponent>();
        });
    }
}



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



void ComponentRegistryTest::test_02_adding_a_factory_succeeds()
{
    ComponentRegistry uut;

    COMPARE(addSampleComponentFactoryTo(uut), true, "adding a factory works");
}



void ComponentRegistryTest::test_03_adding_a_component_twice_fails()
{
    ComponentRegistry uut;

    COMPARE(addSampleComponentFactoryTo(uut), true, "adding 1st factory works");
    COMPARE(addSampleComponentFactoryTo(uut), false, "adding 2nd factory is rejected");
}


void ComponentRegistryTest::test_04_querying_known_component_factory_returns_functor_that_creates_a_component()
{
    ComponentRegistry uut;
    COMPARE(addSampleComponentFactoryTo(uut), true, "adding factory works");

    auto const factory = uut.getFactoryFor(Constants::componentId());
    VERIFY(factory != nullptr, "Ensure we got a factory");

    EXECUTE(auto const component = factory();, "Ensure factory can be invoked");
    VERIFY(std::dynamic_pointer_cast<IComponent>(component) != nullptr, "ensure we got a IComponent from the factory");
}
