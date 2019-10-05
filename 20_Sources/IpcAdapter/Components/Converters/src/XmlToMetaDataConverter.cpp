#include "XmlToMetaDataConverter.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IConfigurable.h"

#include <QStack>
#include <QString>
#include <QXmlSimpleReader>



using IpcAdapter::Components::Converters::XmlToMetaDataConverter;
using IpcAdapter::Core::IPipelineFrame;
using IpcAdapter::Core::PipelineFramePtr;



namespace
{
    bool parseBoolString(QString const& aValue, bool& aFlagToUpdate)
    {
        if (aValue == "true" || aValue == "false")
        {
            aFlagToUpdate = aValue == "true";
            return true;
        }

        return false;
    }

    struct XmlToMetaDataMapper: QXmlDefaultHandler
    {
        XmlToMetaDataMapper()
        {
            // add empty prefix for root element - makes us avoid conditional
            context.push("");
        }

        bool startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes& atts) override
        {
            auto const& prefix = qName + ".";

            for (auto i = 0; i < atts.count(); ++i)
            {
                metaData.insert(context.top() + prefix + atts.qName(i), atts.value(i));
            }

            context.push(context.top() + prefix);

            return true;
        }

        bool endElement(const QString&, const QString&, const QString&) override
        {
            context.pop();
            return true;
        }

        QStack<QString> context;

        IPipelineFrame::MetaDataMap metaData;
    };
}



struct XmlToMetaDataConverter::Data
    : Core::IConfigurable
{
    void onConfigureBegin() override
    {
        forwardRawData = false;
        filterOnBrokenXml = true;
    }

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        REALIZE_REQUIREMENT("R-IPCA-XML2METACNV-002");

        if (aKey == "forward-raw-data")
        {
            return parseBoolString(aValue, forwardRawData);
        }

        if (aKey == "filter-on-broken-xml")
        {
            return parseBoolString(aValue, filterOnBrokenXml);
        }

        return false;
    }

    bool onConfigureEnd() override
    {
        return true;
    }

    XmlToMetaDataMapper mapper;

    bool forwardRawData = false;
    bool filterOnBrokenXml = true;
};



XmlToMetaDataConverter::XmlToMetaDataConverter(): d(std::make_unique<Data>())
{
    REALIZE_REQUIREMENT("R-IPCA-XML2METACNV-001");
}
XmlToMetaDataConverter::~XmlToMetaDataConverter() = default;



IpcAdapter::Core::IConfigurable* XmlToMetaDataConverter::get() const
{
    return d.get();
}



PipelineFramePtr XmlToMetaDataConverter::convert(PipelineFramePtr const& anInput, bool& aStopProcessingFlag)
{
    // prepare next parsing
    d->mapper.metaData.clear();
    aStopProcessingFlag = false;

    // parse XML and update the handler
    QXmlSimpleReader reader;
    reader.setContentHandler(&d->mapper);
    QXmlInputSource source;
    source.setData(anInput->getData());
    auto const parsedOk = reader.parse(source);

    // publish the parsed XML meta-data
    auto xmlFrame = std::make_shared<Core::SimplePipelineFrame>();
    {
        auto const& metaDataParsed = d->mapper.metaData;

        for (auto const& key : metaDataParsed.keys())
        {
            xmlFrame->updateMetaData(key, metaDataParsed.value(key));
        }
    }

    if (d->filterOnBrokenXml)
    {
        aStopProcessingFlag = !parsedOk;
    }

    if (d->forwardRawData)
    {
        xmlFrame->setData(anInput->getData());
    }

    return std::move(xmlFrame);
}



REGISTER_COMPONENT(XmlToMetaDataConverter)
