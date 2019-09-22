#include "Shared/tst/QTestLibInvocation.h"

#include "ComponentRegistryTest.h"
#include "PipelineTest.h"
#include "RuntimeTest.h"



int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QStringList arguments;
    QStringList filters;
    QString outputDir;

    prepareEnv(argc, argv, outputDir, arguments, filters);

    int result = 0;

    result += runTest<IpcAdapter::Core::ComponentRegistryTest>(outputDir, arguments, filters);
    result += runTest<IpcAdapter::Core::PipelineTest>(outputDir, arguments, filters);
    result += runTest<IpcAdapter::Core::RuntimeTest>(outputDir, arguments, filters);

    return result;
}
