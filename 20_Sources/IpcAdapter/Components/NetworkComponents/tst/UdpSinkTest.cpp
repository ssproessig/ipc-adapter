#include "UdpSinkTest.h"

#include "Components/NetworkComponents/src/UdpSink.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Shared/tst/QTestConvenienceMacros.h"

#include <QNetworkDatagram>
#include <QUdpSocket>

#include <functional>


using IpcAdapter::Components::NetworkComponents::UdpSinkTest;
using IpcAdapter::Components::NetworkComponents::UdpSink;



namespace
{
    void testConfiguration(
        std::function<void(IpcAdapter::Core::IConfigurable&)> configurationCallback,
        bool expectConfigurationComplete,
        char const* const configurationCompleteErrorMessage
    )
    {
        auto uut = std::make_unique<UdpSink>();

        TEST_REQUIREMENT("R-IPCA-UDPSNK-002");
        auto& configurable = *uut->getConfigurable();
        {
            configurable.onConfigureBegin();
            configurationCallback(configurable);
            COMPARE(configurable.onConfigureEnd(), expectConfigurationComplete, configurationCompleteErrorMessage);
        }
    }
}



UdpSinkTest::UdpSinkTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-UDPSNK-001");
}



void UdpSinkTest::test_00_UdpSink_is_registered_in_global_factory_registry()
{
    VERIFY(
        IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor("UdpSink") != nullptr,
        "Ensure the UdpSink is published to global registry"
    );
}



void UdpSinkTest::test_01_configuration_fails_for_unknown_parameter()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("unknown", "value"), false, "using an unknown parameter must fail");
    }, false, "w/o any configured parameter we failed");
}



void UdpSinkTest::test_02_configuration_succeeds_for_known_parameters_w_correct_value()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "12345"), true, "configuration succeeds for valid 'port' value");
        COMPARE(configurable.doConfigure("host", "127.0.0.1"), true, "configuration succeeds for valid 'host' value");
    }, true, "succeed configuration when all mandatory parameters were configured");
}



void UdpSinkTest::test_03_configuration_fails_if_mandatory_parameter_is_missing()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable&) {}, false, "w/o any configured parameter we failed");
}



void UdpSinkTest::test_04_configuration_fails_for_invalid_port()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "affe"), false, "configuration fails for invalid 'port' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void UdpSinkTest::test_05_configuration_fails_for_invalid_host()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("host", ""), false, "configuration fails for invalid 'host' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void UdpSinkTest::test_10_forwarding_pipeline_frame_succeeds_for_configured_sink()
{
    TEST_ANNOTATION("Set up a local UDP listener and ensure that UdpSink forwards pipeline frames to it");
    TEST_REQUIREMENT("R-IPCA-UDPSNK-002");

    QUdpSocket udpSocket;

    QByteArray dataReceivedOnUdpSocket;
    connect(&udpSocket, &QUdpSocket::readyRead, this, [&udpSocket, &dataReceivedOnUdpSocket]()
    {
        auto const datagram = udpSocket.receiveDatagram();
        dataReceivedOnUdpSocket = datagram.data();
    });

    COMPARE(
        udpSocket.bind(QHostAddress::LocalHost, 9876),
        true, "binding to a random port must succeed");

    auto uut = std::make_unique<UdpSink>();
    auto& configurable = *uut->getConfigurable();
    configurable.onConfigureBegin();

    COMPARE(
        configurable.doConfigure("port", QString::number(udpSocket.localPort())),
        true, "use the port of our test listener"
    );
    COMPARE(
        configurable.doConfigure("host", udpSocket.localAddress().toString()),
        true, "use the host of our test listener");
    COMPARE(
        configurable.onConfigureEnd(),
        true, "configuration must have succeeded"
    );

    QByteArray const sampleData{"Test1234"};
    Core::SimplePipelineFrame frame{sampleData};

    COMPARE(
        uut->process(frame),
        true, "frame shall be processed successfully");

    QTest::qWait(1000);

    COMPARE(dataReceivedOnUdpSocket, sampleData, "received data SHALL match what was processed by sink");
}
