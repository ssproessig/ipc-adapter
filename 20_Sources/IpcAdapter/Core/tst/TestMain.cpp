#include "Shared/tst/QTestLibInvocation.h"

#include "Core/tst/ComponentRegistryTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    runTest<IpcAdapter::Core::ComponentRegistryTest>(outputDir, arguments, filters);

    return 0;
}
