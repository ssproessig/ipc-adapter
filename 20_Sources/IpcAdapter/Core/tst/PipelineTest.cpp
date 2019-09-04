#include "PipelineTest.h"

#include "Core/src/Pipeline.h"
#include "Core/api/IPipelineFrame.h"
#include "Core/api/ISink.h"
#include "Core/api/ISource.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Core/src/RuntimeConfiguration.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Core::Pipeline;
using IpcAdapter::Core::PipelineTest;
using IpcAdapter::Core::IComponent;
using IpcAdapter::Core::IConfigurable;
using IpcAdapter::Core::IPipelineFrame;
using IpcAdapter::Core::RuntimeConfiguration;
using IpcAdapter::Core::SimplePipelineFrame;



namespace
{
    using DataLog = QList<QByteArray>;

    struct RecordingSink
        : IpcAdapter::Core::ISink
    {
        explicit RecordingSink(DataLog& aDataLog, QByteArray const& anId): dataLog(aDataLog), id(anId) {}

        IConfigurable* getConfigurable() override
        {
            return nullptr;
        }

        bool process(IPipelineFrame const& aPipelineFrame) override
        {
            QString metaDataStr;
            auto const& m = aPipelineFrame.getMetaData();

            for (auto const& key : m.keys())
            {
                metaDataStr = metaDataStr.append(":").append(key).append("=").append(m.value(key).toString());
            }

            QByteArray d(id);
            dataLog.append(d.append(aPipelineFrame.getData()).append(metaDataStr));
            return true;
        }

        DataLog& dataLog;
        QByteArray id;
    };

    struct DummySource: IpcAdapter::Core::ISource
    {
        IConfigurable* getConfigurable() override
        {
            return nullptr;
        }

        void sourceTo(IpcAdapter::Core::IPipelineStep* aPipelineStep) override
        {
            target = aPipelineStep;
        }

        void trigger()
        {
            auto const pipelineFrame = std::make_shared<SimplePipelineFrame>("1234");
            pipelineFrame->updateMetaData("metaKey", 123);
            target->process(pipelineFrame);
        }

        IpcAdapter::Core::IPipelineStep* target = nullptr;
    };
}



PipelineTest::PipelineTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
}



void PipelineTest::test_01_source_to_multiple_sinks()
{
    DataLog dataLog;
    Pipeline uut;

    auto const recordingSink1 = std::make_shared<RecordingSink>(dataLog, "sink1:");
    auto const recordingSink2 = std::make_shared<RecordingSink>(dataLog, "sink2:");

    uut.addSink(recordingSink1.get());
    uut.addSink(recordingSink2.get());

    auto const pipelineFrame = std::make_shared<SimplePipelineFrame>("1234");
    uut.process(pipelineFrame);

    COMPARE(dataLog.count(), 2, "Expect to have recorded one event per sink");
    COMPARE(dataLog.at(0), QByteArray("sink1:1234"), "");
    COMPARE(dataLog.at(1), QByteArray("sink2:1234"), "");
}



void PipelineTest::test_02_dont_create_multiplex_for_unknown_source()
{
    RuntimeConfiguration config;

    EXPECT_EXCEPTION(
        config.getSourceMultiplexFor("source");,
        "without configuring a 'source' there is no multiplex",
        "trying to multiplex unknown source 'source'!"
    )
}



void PipelineTest::test_03_known_source_returns_the_same_multiplex_all_the_time()
{
    RuntimeConfiguration config;

    auto const source = std::make_shared<DummySource>();
    config.addComponent("source", source);

    EXPECT_NO_EXCEPTION(
        config.getSourceMultiplexFor("source");,
        "querying a known source shall not throw")

    COMPARE(
        config.getSourceMultiplexFor("source"),
        config.getSourceMultiplexFor("source"),
        "shall return the same multiplex everytime"
    );
}



void PipelineTest::test_04_source_multiplex_uses_all_pipelines()
{
    RuntimeConfiguration config;

    auto const source = std::make_shared<DummySource>();
    config.addComponent("source", source);

    auto multiplex = config.getSourceMultiplexFor("source");

    Pipeline pipeline1;
    multiplex->sourceTo(&pipeline1);

    Pipeline pipeline2;
    multiplex->sourceTo(&pipeline2);

    DataLog dataLog;
    auto const recordingSink = std::make_shared<RecordingSink>(dataLog, "sink:");
    pipeline1.addSink(recordingSink.get());
    pipeline2.addSink(recordingSink.get());

    source->trigger();

    COMPARE(dataLog.count(), 2, "Expect to have recorded one event per sink AND have the meta-data as well");
    COMPARE(dataLog.at(0), QByteArray("sink:1234:metaKey=123"), "");
    COMPARE(dataLog.at(1), QByteArray("sink:1234:metaKey=123"), "");
}
