#include "Shared/tst/QTestLibInvocation.h"

#include "Components/Sources/tst/UdpSourceTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    runTest<IpcAdapter::Components::Sources::UdpSourceTest>(outputDir, arguments, filters);

    return 0;
}
