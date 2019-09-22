#include "Shared/tst/QTestLibInvocation.h"

#include "Components/Sinks/tst/AmqpExchangeSinkTest.h"
#include "Components/Sinks/tst/UdpSinkTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    int result = 0;

    result += runTest<IpcAdapter::Components::Sinks::AmqpExchangeSinkTest>(outputDir, arguments, filters);
    result += runTest<IpcAdapter::Components::Sinks::UdpSinkTest>(outputDir, arguments, filters);

    return result;
}
