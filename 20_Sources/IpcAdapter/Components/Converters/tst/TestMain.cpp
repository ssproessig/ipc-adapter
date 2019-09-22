#include "Shared/tst/QTestLibInvocation.h"

#include "Components/Converters/tst/ReverseBytesConverterTest.h"
#include "Components/Converters/tst/XmlToMetaDataConverterTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    int result = 0;

    result += runTest<IpcAdapter::Components::Converters::ReverseBytesConverterTest>(outputDir, arguments, filters);
    result += runTest<IpcAdapter::Components::Converters::XmlToMetaDataConverterTest>(outputDir, arguments, filters);

    return result;
}
