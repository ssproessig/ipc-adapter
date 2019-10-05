#include "Shared/tst/QTestLibInvocation.h"

#include "Components/AmqpComponents/tst/AmqpExchangeSinkTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    int result = 0;

    result += runTest<IpcAdapter::Components::AmqpComponents::AmqpExchangeSinkTest>(outputDir, arguments, filters);

    return result;
}
