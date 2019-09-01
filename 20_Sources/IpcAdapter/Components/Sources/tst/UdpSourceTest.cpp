#include "UdpSourceTest.h"

#include "Components/Sources/src/UdpSource.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineStep.h"
#include "Shared/tst/QTestConvenienceMacros.h"

#include <QUdpSocket>



using IpcAdapter::Components::Sources::UdpSourceTest;
using IpcAdapter::Components::Sources::UdpSource;

using UdpSourcePtr = std::shared_ptr<UdpSource>;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QByteArray, sampleData, { "Test123" });
    }



    void testConfiguration(
        UdpSourcePtr const& uut,
        std::function<void(IpcAdapter::Core::IConfigurable&)> configurationCallback,
        bool expectConfigurationComplete,
        char const* const configurationCompleteErrorMessage
    )
    {
        TEST_REQUIREMENT("R-IPCA-SOURCE-002");
        auto& configurable = *uut->getConfigurable();
        {
            configurable.onConfigureBegin();
            configurationCallback(configurable);
            COMPARE(configurable.onConfigureEnd(), expectConfigurationComplete, configurationCompleteErrorMessage);
        }
    }

    void sendDataToUutAssertingDataReceived(UdpSourcePtr const& uut, QHostAddress const& aHostAddress, quint16 aPort)
    {
        struct RecordingPipelineStep : IpcAdapter::Core::IPipelineStep
        {
            QList<QByteArray> data;

            bool process(IpcAdapter::Core::PipelineFramePtr const& aPipelineFrame) override
            {
                data.append(aPipelineFrame->getData());
                return true;
            }

        };

        RecordingPipelineStep pipelineStep;
        uut->sourceTo(&pipelineStep);

        QUdpSocket socket;
        auto const bytesWritten = socket.writeDatagram(Constants::sampleData(), aHostAddress, aPort);
        COMPARE(bytesWritten, Constants::sampleData().length(), "ensure all bytes were written");

        QTest::qWait(250);

        COMPARE(pipelineStep.data.count(), 1, "Ensure we received a message");
        COMPARE(pipelineStep.data.at(0), Constants::sampleData(), "Ensure we received correct data");
    }
}


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



void UdpSourceTest::test_01_configuration_fails_for_unknown_parameter()
{
    testConfiguration(std::make_shared<UdpSource>(), [](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("unknown", "value"), false, "using an unknown parameter must fail");
    }, true, "w/o configuring a known parameter wrongly we succeed");
}



void UdpSourceTest::test_02_configuration_succeeds_for_known_parameters_w_correct_value()
{
    testConfiguration(std::make_shared<UdpSource>(), [](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "12345"), true, "configuration succeeds for valid 'port' value");
        COMPARE(configurable.doConfigure("host", "127.0.0.1"), true, "configuration succeeds for valid 'host' value");
    }, true, "succeed configuration when all mandatory parameters were configured");
}



void UdpSourceTest::test_04_configuration_fails_for_invalid_port()
{
    testConfiguration(std::make_shared<UdpSource>(), [](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "affe"), false, "configuration fails for invalid 'port' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void UdpSourceTest::test_05_configuration_fails_for_invalid_host()
{
    testConfiguration(std::make_shared<UdpSource>(), [](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("host", ""), false, "configuration fails for invalid 'host' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void UdpSourceTest::test_10_forwarding_to_pipeline_succeeds_for_default_configuration()
{
    TEST_REQUIREMENT("R-IPCA-SOURCE-003");

    auto const uut = std::make_shared<UdpSource>();
    testConfiguration(uut, [](IpcAdapter::Core::IConfigurable&) {}, true, "");
    sendDataToUutAssertingDataReceived(uut, QHostAddress("127.0.0.1"), 6789);
}



void UdpSourceTest::test_11_forwarding_to_pipeline_succeeds_for_configured_parameters()
{
    auto const uut = std::make_shared<UdpSource>();
    testConfiguration(uut, [](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "12345"), true, "configuration succeeds for valid 'port' value");
        COMPARE(configurable.doConfigure("host", "127.0.0.1"), true, "configuration succeeds for valid 'host' value");
    }, true, "");
    sendDataToUutAssertingDataReceived(uut, QHostAddress("127.0.0.1"), 12345);
}
