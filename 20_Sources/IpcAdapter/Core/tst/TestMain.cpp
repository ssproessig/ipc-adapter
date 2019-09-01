#include "Shared/tst/QTestLibInvocation.h"

#include "ComponentRegistryTest.h"
#include "RuntimeTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    runTest<IpcAdapter::Core::ComponentRegistryTest>(outputDir, arguments, filters);
    runTest<IpcAdapter::Core::RuntimeTest>(outputDir, arguments, filters);

    return 0;
}
