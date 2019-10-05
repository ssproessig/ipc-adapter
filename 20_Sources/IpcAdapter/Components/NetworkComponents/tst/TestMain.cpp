#include "Shared/tst/QTestLibInvocation.h"

#include "Components/NetworkComponents/tst/UdpSinkTest.h"
#include "Components/NetworkComponents/tst/UdpSourceTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    int result = 0;

    result += runTest<IpcAdapter::Components::NetworkComponents::UdpSinkTest>(outputDir, arguments, filters);
    result += runTest<IpcAdapter::Components::NetworkComponents::UdpSourceTest>(outputDir, arguments, filters);

    return result;
}
